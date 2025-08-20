/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// Military theme colors
# define MILITARY_GREEN "\033[38;5;22m"
# define MILITARY_AMBER "\033[38;5;214m"
# define MILITARY_RED "\033[38;5;196m"
# define MATRIX_GREEN "\033[38;5;46m"

typedef enum e_render_mode
{
	RENDER_NONE = 0,
	RENDER_SIMPLE,
	RENDER_FANCY,
	RENDER_MILITARY,
	RENDER_MATRIX,
	RENDER_HACKER,
}	t_render_mode;

/* Prompt color palette for powerline segments */
typedef enum e_prompt_color
{
	CLR_USER_BG = 25,
	CLR_USER_FG = 231,
	CLR_CWD_BG  = 31,
	CLR_CWD_FG  = 16,
	CLR_GIT_BG  = 90,
	CLR_GIT_FG  = 231,
	CLR_STAT_FG = 231,
	CLR_OK_BG   = 34,
	CLR_ERR_BG  = 160,
	CLR_TIME_BG = 236,
	CLR_TIME_FG = 250
}	t_prompt_color;

/* Buffers for prompt formatting */
typedef struct s_prompt_bufs
{
	char	cwd[PATH_MAX];
	char	timebuf[32];
	char	user[64];
	char	branch[128];
	int		changes;
}	t_prompt_bufs;

#include <sys/stat.h>
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

/* Readline ignore markers for non-printing sequences */
#ifndef RL_IGN_START
# define RL_IGN_START "\001"
#endif
#ifndef RL_IGN_END
# define RL_IGN_END   "\002"
#endif

/* Prompt helper function prototypes */
int		get_term_cols(void);
void	get_time_str(char *dst, size_t sz);
void	get_user_str(char *dst, size_t sz);
void	get_cwd_str(char *dst, size_t sz);

/* run a shell command and capture stdout into buf (NUL-terminated); returns bytes read or -1 */
int   sh_capture(const char *cmd, char *buf, size_t bufsz);

void append_pl_end_to_default(char *dst, size_t dstsz, size_t *off, int last_bg);
void append_pl_seg(char *dst, size_t dstsz, size_t *off,
                   int prev_bg, int bg, int fg, const char *text);

char	*build_prompt(void);

char *get_git_branch(void);
int   get_git_status(void);

#endif