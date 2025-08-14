/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metadata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:50:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 18:02:01 by syzygy           ###   ########.fr       */
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

/* ANSI colors */
static const char *C_RESET  = "\x1b[0m";
static const char *C_BLUE   = "\x1b[34m";
static const char *C_CYAN   = "\x1b[36m";
static const char *C_GREEN  = "\x1b[32m";
static const char *C_YELLOW = "\x1b[33m";
static const char *C_MAG    = "\x1b[35m";
static const char *C_RED    = "\x1b[31m";

extern render_mode_t g_render_mode; /* declared in render.h */
extern int           g_last_status; /* last command exit status */

static char *get_git_branch(void)
{
	FILE *fp;
	char buf[256];

	fp = popen("git branch --show-current 2>/dev/null", "r");
	if (!fp)
		return NULL;
	if (!fgets(buf, sizeof(buf), fp))
	{
		pclose(fp);
		return NULL;
	}
	pclose(fp);
	/* strip trailing newline */
	char *nl = strchr(buf, '\n');
	if (nl)
		*nl = '\0';
	if (buf[0] == '\0')
		return NULL;
	return strdup(buf);
}

static int get_git_status(void)
{
	FILE *fp;
	char buf[64];
	long changes = -1;

	fp = popen("git status --porcelain 2>/dev/null | wc -l", "r");
	if (!fp)
		return -1;
	if (fgets(buf, sizeof(buf), fp))
	{
		char *endptr;
		changes = strtol(buf, &endptr, 10);
		if (endptr == buf)
			changes = -1;
	}
	pclose(fp);
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

	/* optional: clear the screen before showing prompt if user enabled it */
	if (getenv("MS_CLEAR_OUTPUT") && strcmp(getenv("MS_CLEAR_OUTPUT"), "1") == 0)
	{
		/* clear screen and move cursor to top-left */
		printf("\x1b[2J\x1b[H");
		fflush(stdout);
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

	/* Fancy two-line prompt with colors, glyphs and a status symbol */
	{
		const char *left_open  = "╭─";
		const char *left_close = "";
		const char *right_sep  = "";
		const char *line2      = "╰─ ";

		/* status glyph: green check on success, red cross on failure */
		char status_sym[32] = "";
		if (g_last_status == 0)
			ft_snprintf(status_sym, sizeof(status_sym), "%s✔%s", C_GREEN, C_RESET);
		else
			ft_snprintf(status_sym, sizeof(status_sym), "%s✘%s", C_RED, C_RESET);

		/* build change count string */
		char change_s[64] = "";
		if (changes > 0)
			ft_snprintf(change_s, sizeof(change_s), " %s*%d%s", C_YELLOW, changes, C_RESET);

		/* Compose the first (fancy) line
		   Format roughly:
		     ╭─[user cwd]  on [branch][changes] [status]  at [time] 
		   and a second minimal line prefix.
		*/
		if (branch)
		{
			ft_snprintf(tmp, sizeof(tmp),
				"%s%s%s %s%s%s %s %s on %s%s%s%s %s at %s%s %s\n%s",
				left_open,
				C_CYAN, user ? user : "user", C_RESET,
				C_BLUE, cwd, C_RESET,
				right_sep,
				C_MAG, branch, C_RESET,
				change_s,
				status_sym,
				C_GREEN, timebuf, C_RESET,
				left_close,
				line2
			);
			free(branch);
		}
		else
		{
			ft_snprintf(tmp, sizeof(tmp),
				"%s%s%s %s%s%s %s %s at %s%s %s\n%s",
				left_open,
				C_CYAN, user ? user : "user", C_RESET,
				C_BLUE, cwd, C_RESET,
				right_sep,
				status_sym,
				C_GREEN, timebuf, C_RESET,
				left_close,
				line2
			);
		}
	}

	return strdup(tmp);
}