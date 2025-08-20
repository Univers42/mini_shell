/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_stats.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:37:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:19:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

int	ft_history_total_bytes(void)
{
	const t_history_state	*st = access_hist_state();
	t_dll_node				*node;
	int						total;

	total = 0;
	if (!st || !st->list)
		return (0);
	node = st->list->head;
	while (node)
	{
		if (node->data)
			total += ft_strlen((char *)node->data) + 1;
		node = node->next;
	}
	return (total);
}

/**
 * @brief builds and returns an array of pointers entries `t_hist_entry **`
 * it gathers  all the entries from our internal history linked lists,
 * allocates an array,, and fills it with pointers
 * to each entry (+ '\0' terminator)
 * it also update the history length in out state struct
 * @return t_hist_entry**
 * 
 * BASICALLY: it just converts our internal_history into
 * an array of t_hist_entry * for easy access and manipulation
 */
t_hist_entry	**ft_history_list(void)
{
	t_history_state			*st;
	t_history_list_state	*stt;
	t_hist_entry			**arr;
	size_t					need;
	int						len;

	st = access_hist_state();
	if (!st || !st->list || st->list->size == 0)
	{
		if (st)
			st->hist_length = 0;
		return (NULL);
	}
	stt = get_history_list_state();
	need = st->list->size + 1;
	if (!ensure_capacity(stt, need, &arr))
	{
		st->hist_length = 0;
		return (NULL);
	}
	len = fill_hist_entry(st->list, arr, need);
	st->hist_length = len;
	return (arr);
}
