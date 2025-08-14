/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metadata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:50:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 20:50:10 by syzygy           ###   ########.fr       */
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
#ifndef BG_CYAN
# define BG_CYAN   "\x1b[46m"
#endif
#ifndef BG_GREEN
# define BG_GREEN  "\x1b[42m"
#endif
#ifndef BG_RED
# define BG_RED    "\x1b[41m"
#endif
#ifndef FG_BLACK
# define FG_BLACK  "\x1b[30m"
#endif
#ifndef FG_WHITE
# define FG_WHITE  "\x1b[97m"
#endif

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
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execve("/bin/sh", argv, environ);
		_exit(EXIT_FAILURE);
	}
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

/* powerline segment helpers with 256-color ANSI for higher contrast */
static void append_pl_seg(char *dst, size_t dstsz, size_t *off,
                          int prev_bg, int bg, int fg, const char *text)
{
	int w;
	if (*off >= dstsz) return;
	if (prev_bg >= 0)
	{
		w = ft_snprintf(dst + *off, dstsz - *off,
		                "\x1b[38;5;%dm\x1b[48;5;%dm" "", prev_bg, bg);
		if (w > 0) *off += (size_t)w;
	}
	else
	{
		w = ft_snprintf(dst + *off, dstsz - *off, "\x1b[48;5;%dm", bg);
		if (w > 0) *off += (size_t)w;
	}
	w = ft_snprintf(dst + *off, dstsz - *off, "\x1b[38;5;%dm %s ", fg, text);
	if (w > 0) *off += (size_t)w;
}

/* Transition from last_bg to terminal default background with a separator. */
static void append_pl_end_to_default(char *dst, size_t dstsz, size_t *off, int last_bg)
{
	int w = ft_snprintf(dst + *off, dstsz - *off,
	                    "\x1b[38;5;%dm\x1b[49m" "" "%s", last_bg, C_RESET);
	if (w > 0) *off += (size_t)w;
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

	if (getenv("MS_CLEAR_OUTPUT") && strcmp(getenv("MS_CLEAR_OUTPUT"), "1") == 0)
	{
		ssize_t wr = write(STDOUT_FILENO, "\x1b[2J\x1b[H", sizeof("\x1b[2J\x1b[H") - 1);
		(void)wr;
	}

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

	/* Fancy two-line prompt with high-contrast powerline segments and colored input prefix */
	{
		/* colored ❯ for input line */
		char line2buf[32];
		if (g_last_status == 0)
			ft_snprintf(line2buf, sizeof(line2buf), "%s❯ %s", C_GREEN, C_RESET);
		else
			ft_snprintf(line2buf, sizeof(line2buf), "%s❯ %s", C_RED, C_RESET);

		/* git change count (plain, merged in branch text) */
		char change_s[32] = "";
		if (changes > 0)
			ft_snprintf(change_s, sizeof(change_s), " *%d", changes);

		/* pretty cwd (with ~) */
		char pcwd[PATH_MAX];
		pretty_cwd_str(cwd, pcwd, sizeof(pcwd));

		/* Choose vivid 256-color palette */
		enum {
			CLR_USER_BG = 25,   /* deep blue */
			CLR_USER_FG = 231,  /* bright white */
			CLR_CWD_BG  = 31,   /* cyan */
			CLR_CWD_FG  = 16,   /* black */
			CLR_GIT_BG  = 90,   /* magenta */
			CLR_GIT_FG  = 231,  /* bright white */
			CLR_OK_BG   = 34,   /* green */
			CLR_ERR_BG  = 160,  /* red */
			CLR_STAT_FG = 231,  /* bright white */
			CLR_TIME_BG = 236,  /* dark gray */
			CLR_TIME_FG = 250   /* light gray */
		};

		/* Left side: username, cwd, [git], status */
		char leftbuf[1024];
		size_t loff = 0;
		int last_bg = -1;

		/* small zsh-like opener */
		int w = ft_snprintf(leftbuf + loff, sizeof(leftbuf) - loff, "╭─ ");
		if (w > 0) loff += (size_t)w;

		/* user */
		append_pl_seg(leftbuf, sizeof(leftbuf), &loff, last_bg, CLR_USER_BG, CLR_USER_FG,
		              user ? user : "user");
		last_bg = CLR_USER_BG;

		/* cwd */
		append_pl_seg(leftbuf, sizeof(leftbuf), &loff, last_bg, CLR_CWD_BG, CLR_CWD_FG, pcwd);
		last_bg = CLR_CWD_BG;

		/* git (if any) */
		if (branch)
		{
			char brtxt[256];
			ft_snprintf(brtxt, sizeof(brtxt), "%s%s", branch, change_s);
			append_pl_seg(leftbuf, sizeof(leftbuf), &loff, last_bg, CLR_GIT_BG, CLR_GIT_FG, brtxt);
			last_bg = CLR_GIT_BG;
			free(branch);
		}

		/* status (reflect actual exit code) */
		char stat_text[32];
		int  stat_bg = (g_last_status == 0) ? CLR_OK_BG : CLR_ERR_BG;
		if (g_last_status == 0)
			ft_snprintf(stat_text, sizeof(stat_text), "✔ OK");
		else if (g_last_status >= 128)
			ft_snprintf(stat_text, sizeof(stat_text), "✘ SIG(%d)", g_last_status - 128);
		else
			ft_snprintf(stat_text, sizeof(stat_text), "✘ %d", g_last_status);
		append_pl_seg(leftbuf, sizeof(leftbuf), &loff, last_bg, stat_bg, CLR_STAT_FG, stat_text);
		last_bg = stat_bg;

		/* Right side: time segment (right-aligned) */
		char rseg[256];
		size_t roff = 0;
		int r_last_bg = last_bg;

		append_pl_seg(rseg, sizeof(rseg), &roff, r_last_bg, CLR_TIME_BG, CLR_TIME_FG, "at");
		/* add HH:MM:SS inside same time segment */
		w = ft_snprintf(rseg + roff, sizeof(rseg) - roff, "%s %s ", "", timebuf);
		if (w > 0) roff += (size_t)w;

		/* finish time with transition to default background */
		append_pl_end_to_default(rseg, sizeof(rseg), &roff, CLR_TIME_BG);

		/* pad with spaces so time sticks to the right edge */
		int cols = term_cols();
		size_t lvis = visible_len(leftbuf);
		size_t rvis = visible_len(rseg);
		size_t pad = 1;
		if (cols > 0 && (size_t)cols > lvis + rvis)
			pad = (size_t)cols - lvis - rvis;

		char padbuf[256];
		size_t i = 0, maxp = sizeof(padbuf) - 1;
		while (i < pad && i < maxp) { padbuf[i++] = ' '; }
		padbuf[i] = '\0';

		/* compose final two-line prompt */
		ft_snprintf(tmp, sizeof(tmp), "%s%s%s\n%s", leftbuf, padbuf, rseg, line2buf);
	}

	return strdup(tmp);
}