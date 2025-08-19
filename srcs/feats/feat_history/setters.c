/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:54:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/19 16:09:22 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/* small helpers extracted from api_add to satisfy norm limits */
static int	ensure_initialized(t_history_state *st)
{
	if (!st)
		return (0);
	if (!st->initialized)
	{
		if (api_init(NULL, NULL) != 0)
			return (0);
	}
	return (1);
}

static int	is_blank_line(const char *line)
{
	const char	*p;

	if (!line)
		return (1);
	p = line;
	while (*p && ft_isspace((unsigned char)*p))
		p++;
	return (*p == '\0');
}

static int	is_duplicate_last(const char *line)
{
	int				n;
	t_hist_entry	*last;

	n = g_custom_history_length;
	if (n <= 0)
		return (0);
	last = custom_history_get(n);
	if (last && last->line && ft_strcmp(last->line, line) == 0)
		return (1);
	return (0);
}

/**
 * lazy init to ensure we always record 
 * skip whitespace only
 * prevent consecutive duplicates
 */
void	api_add(const char *line)
{
	t_history_state	*st;

	st = S();
	if (!st || !line || !*line)
		return ;
	if (!ensure_initialized(st))
		return ;
	if (is_blank_line(line))
		return ;
	if (is_duplicate_last(line))
		return ;
	(dll_push_tail_line(line), add_history(line));
	if (st->histsize > 0)
		custom_stifle_history(st->histsize);
}
