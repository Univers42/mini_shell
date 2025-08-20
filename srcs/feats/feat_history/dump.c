/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:03:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:19:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/* renamed from `fallback` for clearer intent */
static char	**dump_from_list(void)
{
	t_history_state	*st;
	size_t			m;
	t_dll_node		*node;
	char			**out;

	st = access_hist_state();
	if (!st || !st->list || st->list->size == 0)
	{
		out = (char **)malloc(sizeof(char *));
		if (out)
			out[0] = NULL;
		return (out);
	}
	m = st->list->size;
	node = st->list->head;
	return (dup_list_to_array(node, m));
}

char	**api_dump(void)
{
	t_hist_entry	**harr;
	int				n;
	t_history_state	*st;

	harr = ft_history_list();
	st = access_hist_state();
	if (st)
		n = st->hist_length;
	else
		n = 0;
	if (harr && n > 0)
		return (dup_history_entries(harr, n));
	return (dump_from_list());
}

