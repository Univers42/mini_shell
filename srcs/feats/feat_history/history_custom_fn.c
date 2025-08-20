/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_ft_fn.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 23:40:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:09:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

int	ensure_capacity(t_history_list_state *stt, size_t need,
								t_hist_entry ***out)
{
	t_hist_entry	**arr;
	size_t			asz;

	arr = *(stt->hist_array_ptr);
	asz = *(stt->array_size_ptr);
	if (asz >= need)
	{
		*out = arr;
		return (1);
	}
	return (reset_and_alloc_array(stt, need, out));
}

static t_hist_entry	*alloc_hist_entry(const char *line)
{
	t_hist_entry	*entry;

	entry = (t_hist_entry *)malloc(sizeof(t_hist_entry));
	if (!entry)
		return (NULL);
	entry->line = (char *)line;
	entry->timestamp = NULL;
	entry->data = NULL;
	return (entry);
}

int	fill_hist_entry(t_doubly_list *list, t_hist_entry **arr, size_t cap)
{
	t_dll_node	*node;
	size_t		i;

	node = list->head;
	i = -1;
	while (node && ++i + 1 < cap)
	{
		if (!arr[i])
		{
			arr[i] = alloc_hist_entry(node->data);
			if (!arr[i])
				break ;
		}
		node = node->next;
	}
	if (i < cap)
		arr[i] = NULL;
	return ((int)i);
}
