/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metadata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:50:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 20:25:48 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"
#include <time.h>
#include <sys/ioctl.h>

extern char **environ;

extern render_mode_t g_render_mode; /* declared in render.h */
extern int           g_last_status; /* last command exit status */

/* ANSI colors (use macros so symbols are available at compile-time) */
#ifndef C_RESET
# define C_RESET  "\x1b[0m"
#endif
#ifndef C_BLUE
# define C_BLUE   "\x1b[34m"
#endif
#ifndef C_CYAN
# define C_CYAN   "\x1b[36m"
#endif
#ifndef C_GREEN
# define C_GREEN  "\x1b[32m"
#endif
#ifndef C_YELLOW
# define C_YELLOW "\x1b[33m"
#endif
#ifndef C_MAG
# define C_MAG    "\x1b[35m"
#endif
#ifndef C_RED
# define C_RED    "\x1b[31m"
#endif

/* background/foreground helpers (basic 8-color ANSI) */
#ifndef BG_BLUE
# define BG_BLUE   "\x1b[44m"
#endif
#ifndef BG_MAG
# define BG_MAG    "\x1b[45m"
#endif
#ifndef BG_DEF
# define BG_DEF    "\x1b[49m"
#endif
#ifndef FG_BLACK
# define FG_BLACK  "\x1b[30m"
#endif
#ifndef FG_WHITE
# define FG_WHITE  "\x1b[97m"
#endif

/* helpers to improve zsh-like rendering */
static int term_cols(void)
{
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
		return (int)ws.ws_col;
	return 0;
}

/* compute visible length ignoring ANSI escape sequences */
static size_t visible_len(const char *s)
{
	size_t n = 0;
	while (*s)
	{
		if ((unsigned char)*s == 0x1b) {
			/* skip CSI sequence: ESC '[' ... 'm' (best-effort) */
			s++;
			if (*s == '[') {
				s++;
				while (*s && *s != 'm')
					s++;
				if (*s == 'm')
					s++;
			}
			continue;
		}
		n++;
		s++;
	}
	return n;
}

/* replace $HOME prefix with ~ for prettier cwd */
static void pretty_cwd_str(const char *cwd, char *out, size_t outsz)
{
	const char *home = getenv("HOME");
	if (home && *home && strncmp(cwd, home, strlen(home)) == 0)
		ft_snprintf(out, outsz, "~%s", cwd + strlen(home));
	else
		ft_snprintf(out, outsz, "%s", cwd);
}

static char *get_git_branch(void)
{
	int		pipefd[2];
	pid_t	pid;
	char	buf[256];
	ssize_t	n;
	char	*out;
	const char *cmd = "git branch --show-current 2>/dev/null";
	char *const argv[] = { (char *)"sh", (char *)"-c", (char *)cmd, NULL };

	if (pipe(pipefd) == -1)
		return NULL;
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return NULL;
	}
	if (pid == 0)
	{
		/* child */
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execve("/bin/sh", argv, environ);
		_exit(EXIT_FAILURE);
	}
	/* parent */
	close(pipefd[1]);
	n = read(pipefd[0], buf, sizeof(buf) - 1);
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
	if (n <= 0)
		return NULL;
	buf[n] = '\0';
	{
		char *nl = strchr(buf, '\n');
		if (nl)
			*nl = '\0';
	}
	if (buf[0] == '\0')
		return NULL;
	out = strdup(buf);
	return out;
}

static int get_git_status(void)
{
	int		pipefd[2];
	pid_t	pid;
	char	buf[64];
	ssize_t	n;
	long	changes = -1;
	const char *cmd = "git status --porcelain 2>/dev/null | wc -l";
	char *const argv[] = { (char *)"sh", (char *)"-c", (char *)cmd, NULL };

	if (pipe(pipefd) == -1)
		return -1;
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return -1;
	}
	if (pid == 0)
	{
		/* child */
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execve("/bin/sh", argv, environ);
		_exit(EXIT_FAILURE);
	}
	/* parent */
	close(pipefd[1]);
	n = read(pipefd[0], buf, sizeof(buf) - 1);
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
	if (n > 0)
	{
		char *endptr;
		buf[n] = '\0';
		changes = strtol(buf, &endptr, 10);
		if (endptr == buf)
			changes = -1;
	}
	return (int)changes;
}

char *build_prompt(void)
{
	char cwd[PATH_MAX];
	char tmp[1024];
	char timebuf[64];
	char *branch = NULL;
	int changes = -1;
	const char *user = getenv("USER");
	time_t now;
	struct tm *tm;
	char pcwd[PATH_MAX]; /* pretty cwd */

	/* optional: clear the screen before showing prompt if user enabled it */
	if (getenv("MS_CLEAR_OUTPUT") && strcmp(getenv("MS_CLEAR_OUTPUT"), "1") == 0)
	{
		/* clear screen and move cursor to top-left using write() to avoid stdio buffering */
		ssize_t wr = write(STDOUT_FILENO, "\x1b[2J\x1b[H", sizeof("\x1b[2J\x1b[H") - 1);
		(void)wr;
	}

	/* gather data */
	branch = get_git_branch();
	changes = get_git_status();
	if (!getcwd(cwd, sizeof(cwd)))
		ft_snprintf(cwd, sizeof(cwd), "?");

	time(&now);
	tm = localtime(&now);
	if (tm)
		strftime(timebuf, sizeof(timebuf), "%H:%M:%S", tm);
	else
		ft_snprintf(timebuf, sizeof(timebuf), "--:--:--");

	/* If simple mode or any helper failed, fallback to compact prompt */
	if (g_render_mode != RENDER_FANCY)
	{
		if (branch)
		{
			if (changes > 0)
				ft_snprintf(tmp, sizeof(tmp), "%s on %s *%d $ ", cwd, branch, changes);
			else
				ft_snprintf(tmp, sizeof(tmp), "%s on %s $ ", cwd, branch);
			free(branch);
		}
		else
			ft_snprintf(tmp, sizeof(tmp), "%s $ ", cwd);
		return strdup(tmp);
	}

	/* Fancy two-line prompt with powerline-like segments and a colored input prefix */
	{
		const char *left_open = "╭─";

		/* colored ❯ for input line */
		char line2buf[32];
		if (g_last_status == 0)
			ft_snprintf(line2buf, sizeof(line2buf), "%s❯ %s", C_GREEN, C_RESET);
		else
			ft_snprintf(line2buf, sizeof(line2buf), "%s❯ %s", C_RED, C_RESET);

		/* status glyph: green check on success, red cross on failure */
		char status_sym[32] = "";
		if (g_last_status == 0)
			ft_snprintf(status_sym, sizeof(status_sym), "%s✔%s", C_GREEN, C_RESET);
		else
			ft_snprintf(status_sym, sizeof(status_sym), "%s✘%s", C_RED, C_RESET);

		/* git change count string */
		char change_s[64] = "";
		if (changes > 0)
			ft_snprintf(change_s, sizeof(change_s), " %s*%d%s", C_YELLOW, changes, C_RESET);

		/* pretty cwd (with ~) */
		pretty_cwd_str(cwd, pcwd, sizeof(pcwd));

		/* decorative middle bar (static for now) */
		const char *bar = "▓▒░·························░▒▓";

		/* left-side segments with backgrounds and separators:
		   include username before cwd for a zsh-like look
		*/
		char leftbuf[640];
		if (branch)
		{
			ft_snprintf(leftbuf, sizeof(leftbuf),
				"%s"                             /* ╭─ */
				" %s "                           /* username */
				" " BG_BLUE FG_WHITE " %s " C_RESET /* cwd segment */
				"  on "
				BG_MAG FG_WHITE "%s" C_RESET "%s "   /* branch + changes */
				"%s %s",                           /* bar + status */
				left_open,
				C_CYAN, user ? user : "user", C_RESET,
				pcwd,
				branch, change_s,
				bar, status_sym
			);
			free(branch);
		}
		else
		{
			ft_snprintf(leftbuf, sizeof(leftbuf),
				"%s %s"
				" " BG_BLUE FG_WHITE " %s " C_RESET
				" %s %s",
				left_open,
				C_CYAN, user ? user : "user", C_RESET,
				pcwd,
				bar, status_sym
			);
		}

		/* right-aligned time segment: "  at HH:MM:SS " */
		char rseg[96];
		ft_snprintf(rseg, sizeof(rseg), "  at %s ", timebuf);

		/* pad with dots so time is right-aligned */
		int cols = term_cols();
		size_t lvis = visible_len(leftbuf);
		size_t rvis = visible_len(rseg);
		size_t pad = 1;
		if (cols > 0 && (size_t)cols > lvis + rvis)
			pad = (size_t)cols - lvis - rvis;

		char padbuf[256];
		size_t i = 0, maxp = sizeof(padbuf) - 1;
		while (i < pad && i < maxp) { padbuf[i++] = '.'; }
		padbuf[i] = '\0';

		/* compose the first line, then the colored input prefix line */
		ft_snprintf(tmp, sizeof(tmp), "%s%s%s\n%s", leftbuf, padbuf, rseg, line2buf);
	}

	return strdup(tmp);
}