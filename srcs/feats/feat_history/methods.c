/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:53:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 17:29:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

void	api_shutdown(void)
{
	t_history_state	*st;

	st = S();
	if (!st)
		return ;
	api_save();
	dll_clear_all();
	rl_clear_history();
	if (st->list)
		ft_dll_destroy(st->list);
	ft_memset(st, 0, sizeof(*st));
}

char **api_dump(void)
{
	t_history_state	*st;
	size_t			n;
	size_t			i;
	char			**out;
	t_dll_node		*node;

	st = S();
	if (!st || !st->list)
		return (NULL);
	n = st->list->size;
	out = (char **)malloc((n + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	i = 0;
	node = st->list->head;
	while (i < n && node)
	{
		if (node->data)
		{
			out[i] = ft_strdup((const char *)node->data);
			if (!out[i])
			{
				while (i > 0)
					free(out[--i]);
				return (free(out), NULL);
			}
		}
		else
			out[i] = NULL;
		i++;
		node = node->next;
	}
	out[n] = NULL;
	return (out);
}
