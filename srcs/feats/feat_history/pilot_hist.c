/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pilot_hist.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:24:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:17:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

// Convert from 1-based to 0-based indexing [REPAIR THE ERROR]
t_hist_entry	*ft_history_get(int offset)
{
	t_hist_entry	**list;
	t_history_state	*st;

	if (offset <= 0)
		return (NULL);
	list = ft_history_list();
	st = access_hist_state();
	if (!list || !st || offset > st->hist_length)
		return (NULL);
	return (list[offset - 1]);
}

t_hist_entry	*ft_current_history(void)
{
	t_history_state	*st;

	st = access_hist_state();
	if (!st || st->hist_length <= 0)
		return (NULL);
	return (ft_history_get(st->hist_length));
}

t_hist_entry	*ft_previous_history(void)
{
	t_history_state	*st;

	st = access_hist_state();
	if (!st || st->hist_length <= 1)
		return (NULL);
	return (ft_history_get(st->hist_length - 1));
}

int	ft_where_history(void)
{
	t_history_state	*st;

	st = access_hist_state();
	if (st && st->hist_length > 0)
		return (st->hist_length - 1);
	return (0);
}

int	ft_history_set_pos(int pos)
{
	t_history_state	*st;

	st = access_hist_state();
	if (!st)
		return (0);
	if (pos < 0 || pos >= st->hist_length)
		return (0);
	return (1);
}
