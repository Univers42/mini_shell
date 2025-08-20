/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metadata.c                                         :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "minishell.h"

/* small helpers */
static void	maybe_clear_output(void)
{
	const char	*v;

	v = getenv("MS_CLEAR_OUTPUT");
	if (v && strcmp(v, "1") == 0)
	{
		ssize_t	wr;

		wr = write(STDOUT_FILENO, "\x1b[2J\x1b[H",
				sizeof("\x1b[2J\x1b[H") - 1);
		(void)wr;
	}
}

static void	get_time_string(char *dst, size_t sz)
{
	time_t		now;
	struct tm	*tmv;

	time(&now);
	tmv = localtime(&now);
	if (tmv)
		strftime(dst, sz, "%H:%M:%S", tmv);
	else
		ft_snprintf(dst, sz, "--:--:--");
}

static int	is_ansi_start(const char *s)
{
	if (!s)
		return (0);
	if ((unsigned char)s[0] != 0x1b)
		return (0);
	if (s[1] != '[')
		return (0);
	return (1);
}

static const char	*skip_ansi(const char *s)
{
	if (!is_ansi_start(s))
		return (s);
	s += 2;
	while (*s && *s != 'm')
		s++;
	if (*s == 'm')
		s++;
	return (s);
}

static int	is_rl_marker(char c)
{
	return (c == '\001' || c == '\002');
}

/* compute visible length ignoring ANSI escape sequences and RL markers */
static size_t	visible_len(const char *s)
{
	size_t	n;

	n = 0;
	while (*s)
	{
		if (is_ansi_start(s))
		{
			s = skip_ansi(s);
			continue ;
		}
		if (is_rl_marker(*s))
		{
			s++;
			continue ;
		}
		n++;
		s++;
	}
	return (n);
}

static int	term_cols(void)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
		return ((int)ws.ws_col);
	return (0);
}

static void	pretty_cwd_str(const char *cwd, char *out, size_t outsz)
{
	const char	*home;

	home = getenv("HOME");
	if (home && *home && strncmp(cwd, home, strlen(home)) == 0)
		ft_snprintf(out, outsz, "~%s", cwd + strlen(home));
	else
		ft_snprintf(out, outsz, "%s", cwd);
}

static void	get_git_info(char **branch_out, int *changes_out)
{
	if (branch_out)
		*branch_out = get_git_branch();
	if (changes_out)
		*changes_out = get_git_status();
}

static void	format_line2buf(char *out, size_t outsz)
{
	if (ms()->last_status == 0)
		ft_snprintf(out, outsz, "%s",
			RL_IGN_START C_GREEN RL_IGN_END "> "
			RL_IGN_START C_RESET RL_IGN_END);
	else
		ft_snprintf(out, outsz, "%s",
			RL_IGN_START C_RED RL_IGN_END "> "
			RL_IGN_START C_RESET RL_IGN_END);
}

static void	change_suffix(int changes, char *out, size_t outsz)
{
	if (changes > 0)
		ft_snprintf(out, outsz, " *%d", changes);
	else if (outsz)
		out[0] = '\0';
}

static int	format_status_text(char *txt, size_t tsz)
{
	int	bg;

	bg = (ms()->last_status == 0) ? 34 : 160;
	if (ms()->last_status == 0)
		ft_snprintf(txt, tsz, "✔ OK");
	else if (ms()->last_status >= 128)
		ft_snprintf(txt, tsz, "✘ SIG(%d)", ms()->last_status - 128);
	else
		ft_snprintf(txt, tsz, "✘ %d", ms()->last_status);
	return (bg);
}

/* returns last_bg used */
static int	build_leftbuf(char *out, size_t outsz,
				const char *user, const char *pcwd,
				const char *branch, int changes)
{
	enum {
		CLR_USER_BG = 25, CLR_USER_FG = 231,
		CLR_CWD_BG = 31, CLR_CWD_FG = 16,
		CLR_GIT_BG = 90, CLR_GIT_FG = 231,
		CLR_STAT_FG = 231
	};
	size_t	loff;
	int		last_bg;
	int		w;

	loff = 0;
	last_bg = -1;
	w = ft_snprintf(out + loff, outsz - loff, "╭─ ");
	if (w > 0)
		loff += (size_t)w;
	append_pl_seg(out, outsz, &loff, last_bg, CLR_USER_BG,
		CLR_USER_FG, user ? user : "user");
	last_bg = CLR_USER_BG;
	append_pl_seg(out, outsz, &loff, last_bg, CLR_CWD_BG,
		CLR_CWD_FG, pcwd);
	last_bg = CLR_CWD_BG;
	if (branch)
	{
		char	brtxt[256];
		char	cs[32];

		cs[0] = '\0';
		change_suffix(changes, cs, sizeof(cs));
		ft_snprintf(brtxt, sizeof(brtxt), "%s%s", branch, cs);
		append_pl_seg(out, outsz, &loff, last_bg,
			CLR_GIT_BG, CLR_GIT_FG, brtxt);
		last_bg = CLR_GIT_BG;
	}
	{
		char	stat_text[32];
		int		stat_bg;

		stat_bg = format_status_text(stat_text, sizeof(stat_text));
		append_pl_seg(out, outsz, &loff, last_bg, stat_bg,
			CLR_STAT_FG, stat_text);
		last_bg = stat_bg;
	}
	return (last_bg);
}

static void	build_rseg(char *out, size_t outsz, int last_bg,
				const char *timebuf)
{
	enum { CLR_TIME_BG = 236, CLR_TIME_FG = 250 };
	size_t	off;
	int		w;

	off = 0;
	append_pl_seg(out, outsz, &off, last_bg, CLR_TIME_BG, CLR_TIME_FG, "at");
	w = ft_snprintf(out + off, outsz - off, " %s ", timebuf);
	if (w > 0)
		off += (size_t)w;
	append_pl_end_to_default(out, outsz, &off, CLR_TIME_BG);
}

static size_t	compute_padding(const char *left, const char *right, int cols)
{
	size_t	lvis;
	size_t	rvis;

	lvis = visible_len(left);
	rvis = visible_len(right);
	if (cols > 0 && (size_t)cols > lvis + rvis)
		return ((size_t)cols - lvis - rvis);
	return (1);
}

static char	*build_simple_prompt(const char *cwd, char *branch, int changes)
{
	char	tmp[1024];

	if (branch)
	{
		if (changes > 0)
			ft_snprintf(tmp, sizeof(tmp), "%s on %s *%d $ ",
				cwd, branch, changes);
		else
			ft_snprintf(tmp, sizeof(tmp), "%s on %s $ ", cwd, branch);
		free(branch);
	}
	else
		ft_snprintf(tmp, sizeof(tmp), "%s $ ", cwd);
	return (strdup(tmp));
}

static char	*build_fancy_prompt(const char *cwd, const char *user,
				char *branch, int changes, const char *timebuf)
{
	char	line2buf[64];
	char	pcwd[PATH_MAX];
	char	leftbuf[1024];
	char	rseg[256];
	char	padbuf[256];
	char	tmp[1024];
	int		last_bg;
	int		cols;
	size_t	pad;
	size_t	i;
	size_t	maxp;

	format_line2buf(line2buf, sizeof(line2buf));
	pretty_cwd_str(cwd, pcwd, sizeof(pcwd));
	last_bg = build_leftbuf(leftbuf, sizeof(leftbuf), user, pcwd, branch,
			changes);
	if (branch)
		free(branch);
	build_rseg(rseg, sizeof(rseg), last_bg, timebuf);
	cols = term_cols();
	pad = compute_padding(leftbuf, rseg, cols);
	maxp = sizeof(padbuf) - 1;
	i = 0;
	while (i < pad && i < maxp)
	{
		padbuf[i] = ' ';
		i++;
	}
	padbuf[i] = '\0';
	ft_snprintf(tmp, sizeof(tmp), "%s%s%s\n%s", leftbuf, padbuf, rseg,
		line2buf);
	return (strdup(tmp));
}

char	*build_prompt(void)
{
	char		cwd[PATH_MAX];
	char		timebuf[64];
	char		*branch;
	int			changes;
	const char	*user;

	branch = NULL;
	changes = -1;
	user = getenv("USER");
	maybe_clear_output();
	get_git_info(&branch, &changes);
	if (!getcwd(cwd, sizeof(cwd)))
		ft_snprintf(cwd, sizeof(cwd), "?");
	get_time_string(timebuf, sizeof(timebuf));
	if (ms()->render_mode != RENDER_FANCY)
		return (build_simple_prompt(cwd, branch, changes));
	return (build_fancy_prompt(cwd, user, branch, changes, timebuf));
}