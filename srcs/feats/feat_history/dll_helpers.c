/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:38:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:10:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

void	dll_push_tail_line(const char *line)
{
	t_history_state	*st;
	char			*dup;
	char			*last;
	char			*old;

	st = access_hist_state();
	if (!st || !st->list || !line || !*line)
		return ;
	if (st->list->size > 0)
	{
		last = (char *)ft_dll_back(st->list);
		if (last && ft_strcmp(last, line) == 0)
			return ;
	}
	dup = ft_strdup(line);
	if (!dup)
		return ;
	if (!ft_dll_push_back(st->list, dup))
		free(dup);
	else if (st->histsize > 0 && st->list->size > (size_t)st->histsize)
	{
		old = (char *)ft_dll_pop_front(st->list);
		free(old);
	}
	update_history_length();
}

void	dll_clear_all(void)
{
	t_history_state		*st;
	char				*s;

	st = access_hist_state();
	if (!st || !st->list)
		return ;
	while (!ft_dll_is_empty(st->list))
	{
		s = (char *)ft_dll_pop_front(st->list);
		free(s);
	}
	update_history_length();
}
