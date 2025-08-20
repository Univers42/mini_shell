/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_limit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:37:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:17:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * @brief stifle_history
 * Limit the history list to a maximum number of entries
 * @max: Maximum number of history entries to keep (must be >= 0)
 * 
 * This function limits the history to at most 'max' entries. If the current
 * history has more than 'max' entries, the oldest entries are removed.
 * If max is 0, history is unlimited (same as calling unstifle_history).
 * 
 * Returns: 0 on success, -1 on error
 */

int	ft_stifle_history(int max)
{
	t_history_state	*st;
	char			*old_entry;

	st = access_hist_state();
	if (!st || !st->list)
		return (-1);
	if (max < 0)
		return (-1);
	if (max == 0)
	{
		st->histsize = 0;
		return (update_history_length(), 0);
	}
	st->histsize = max;
	while (st->list->size > (size_t)max)
	{
		old_entry = (char *)ft_dll_pop_front(st->list);
		if (old_entry)
			free(old_entry);
	}
	update_history_length();
	return (0);
}

/**
 * @brief The purpose of `ft_unstifle_history` is to remove any 
 * limit on the number of history entries, allowing the history to
 * grow without restriction. it sets the limit to zero
 * *(unlimited)
 */
int	ft_unstifle_history(void)
{
	t_history_state	*st;
	int				prev_size;

	st = access_hist_state();
	if (!st)
		return (-1);
	prev_size = st->histsize;
	st->histsize = 0;
	update_history_length();
	if (prev_size > 0)
		return (prev_size);
	return (-1);
}
