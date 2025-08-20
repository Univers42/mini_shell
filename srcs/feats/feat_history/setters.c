/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:54:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:36:13 by dlesieur         ###   ########.fr       */
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

/**
 * lazy init to ensure we always record 
 * skip whitespace only
 * prevent consecutive duplicates
 */
void	api_add(const char *line)
{
	t_history_state	*st;

	st = access_hist_state();
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
		ft_stifle_history(st->histsize);
}
