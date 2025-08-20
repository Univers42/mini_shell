#include "render.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	visible_len(const char *s)
{
	size_t	n;

	n = 0;
	while (*s)
	{
		if ((unsigned char)*s == 0x1b)
		{
			s++;
			if (*s == '[')
			{
				s++;
				while (*s && *s != 'm')
					s++;
				if (*s == 'm')
					s++;
			}
			continue ;
		}
		if (*s == '\001' || *s == '\002')
		{
			s++;
			continue ;
		}
		n++;
		s++;
	}
	return ((int)n);
}

static void	format_status(char *dst, size_t sz, int status)
{
	if (status == 0)
		ft_snprintf(dst, sz, "✔ OK");
	else if (status >= 128)
		ft_snprintf(dst, sz, "✘ SIG(%d)", status - 128);
	else
		ft_snprintf(dst, sz, "✘ %d", status);
}

static void	format_line2(char *dst, size_t sz, int status)
{
	if (status == 0)
	{
		// Add Readline ignore markers and reset color before the input area.
		ft_snprintf(dst, sz,
			"%s>%s ",
			RL_IGN_START C_GREEN RL_IGN_END,
			RL_IGN_START C_RESET RL_IGN_END);
	}
	else
	{
		ft_snprintf(dst, sz,
			"%s>%s ",
			RL_IGN_START C_RED RL_IGN_END,
			RL_IGN_START C_RESET RL_IGN_END);
	}
}

static void	build_left(char *dst, size_t sz, t_prompt_bufs *bufs, int status)
{
	size_t	off;
	int		last_bg;
	char	stat_text[32];

	off = 0;
	last_bg = -1;
	off += ft_snprintf(dst + off, sz - off, "╭─ ");
	append_pl_seg(dst, sz, &off, last_bg, CLR_USER_BG, CLR_USER_FG, bufs->user);
	last_bg = CLR_USER_BG;
	append_pl_seg(dst, sz, &off, last_bg, CLR_CWD_BG, CLR_CWD_FG, bufs->cwd);
	last_bg = CLR_CWD_BG;
	if (bufs->branch[0])
	{
		char	brtxt[128];
		if (bufs->changes > 0)
			ft_snprintf(brtxt, sizeof(brtxt), "%s *%d", bufs->branch, bufs->changes);
		else
			ft_strlcpy(brtxt, bufs->branch, sizeof(brtxt));
		append_pl_seg(dst, sz, &off, last_bg, CLR_GIT_BG, CLR_GIT_FG, brtxt);
		last_bg = CLR_GIT_BG;
	}
	format_status(stat_text, sizeof(stat_text), status);
	append_pl_seg(dst, sz, &off, last_bg,
		status == 0 ? CLR_OK_BG : CLR_ERR_BG, CLR_STAT_FG, stat_text);
	// Ensure prompt ends with a reset so input area is not colored
	off += ft_snprintf(dst + off, sz - off, RL_IGN_START C_RESET RL_IGN_END);
}

static void	build_right(char *dst, size_t sz, int last_bg)
{
	(void)sz;
	(void)last_bg;
	dst[0] = '\0';
}

char	*build_prompt(void)
{
	t_prompt_bufs	bufs;
	char			left[256];
	char			right[128];
	char			line2[32];
	char			tmp[512];
	int				cols;
	int				pad;
	int				last_bg;

	get_user_str(bufs.user, sizeof(bufs.user));
	get_cwd_str(bufs.cwd, sizeof(bufs.cwd));
	bufs.branch[0] = '\0';
	bufs.changes = -1;
	{
		char *b = get_git_branch();
		if (b)
		{
			ft_strlcpy(bufs.branch, b, sizeof(bufs.branch));
			free(b);
		}
		bufs.changes = get_git_status();
	}
	format_line2(line2, sizeof(line2), ms()->last_status);
	build_left(left, sizeof(left), &bufs, ms()->last_status);
	last_bg = (ms()->last_status == 0) ? CLR_OK_BG : CLR_ERR_BG;
	build_right(right, sizeof(right), last_bg); // no timebuf
	cols = get_term_cols();
	pad = cols - visible_len(left) - visible_len(right);
	if (pad < 1)
		pad = 1;
	ft_snprintf(tmp, sizeof(tmp), "%s%*s%s\n%s", left, pad, "", right, line2);
	return (ft_strdup(tmp));
}
