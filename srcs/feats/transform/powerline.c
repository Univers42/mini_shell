/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   powerline.c                                        :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

/* powerline segment helpers with 256-color ANSI for higher contrast */
void	append_pl_seg(char *dst, size_t dstsz, size_t *off,
				int prev_bg, int bg, int fg, const char *text)
{
	int	w;

	if (*off >= dstsz)
		return ;
	if (prev_bg >= 0)
	{
		w = ft_snprintf(
				dst + *off, dstsz - *off,
				RL_IGN_START "\x1b[38;5;%dm" RL_IGN_END
				RL_IGN_START "\x1b[48;5;%dm" RL_IGN_END
				"",
				prev_bg, bg);
		if (w > 0)
			*off += (size_t)w;
	}
	else
	{
		w = ft_snprintf(
				dst + *off, dstsz - *off,
				RL_IGN_START "\x1b[48;5;%dm" RL_IGN_END,
				bg);
		if (w > 0)
			*off += (size_t)w;
	}
	w = ft_snprintf(
			dst + *off, dstsz - *off,
			RL_IGN_START "\x1b[38;5;%dm" RL_IGN_END
			" %s ",
			fg, text);
	if (w > 0)
		*off += (size_t)w;
}

/* Transition from last_bg to terminal default background with a separator. */
void	append_pl_end_to_default(char *dst, size_t dstsz, size_t *off,
				int last_bg)
{
	int	w;

	w = ft_snprintf(
			dst + *off, dstsz - *off,
			RL_IGN_START "\x1b[38;5;%dm" RL_IGN_END
			RL_IGN_START "\x1b[49m" RL_IGN_END
			""
			RL_IGN_START "\x1b[0m" RL_IGN_END,
			last_bg);
	if (w > 0)
		*off += (size_t)w;
}
