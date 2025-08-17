/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:53:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 19:55:36 by dlesieur         ###   ########.fr       */
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
	/* Prefer readline's authoritative history */
	HIST_ENTRY		**harr;
	int				n;
	char			**out;
	int				i;

	harr = history_list();
	n = history_length;
	if (harr && n > 0)
	{
		/* FIX: allocate (n + 1) pointers, not n + sizeof(char*) bytes */
		out = (char **)malloc(((size_t)n + 1u) * sizeof(char *));
		if (!out)
			return (NULL);
		i = 0;
		while (i < n)
		{
			if (harr[i] && harr[i]->line)
				out[i] = ft_strdup(harr[i]->line);
			else
				out[i] = ft_strdup("");
			if (!out[i])
			{
				while (i > 0)
					free(out[--i]);
				return (free(out), NULL);
			}
			i++;
		}
		out[n] = NULL;
		return (out);
	}

	/* Fallback: dump our internal DLL list */
	{
		t_history_state	*st;
		size_t			m;
		size_t			j;
		t_dll_node		*node;

		st = S();
		if (!st || !st->list || st->list->size == 0)
		{
			out = (char **)malloc(sizeof(char *));
			if (out)
				out[0] = NULL;
			return (out);
		}
		m = st->list->size;
		out = (char **)malloc((m + 1) * sizeof(char *));
		if (!out)
			return (NULL);
		j = 0;
		node = st->list->head;
		while (j < m && node)
		{
			if (node->data)
				out[j] = ft_strdup((const char *)node->data);
			else
				out[j] = ft_strdup("");
			if (!out[j])
			{
				while (j > 0)
					free(out[--j]);
				return (free(out), NULL);
			}
			j++;
			node = node->next;
		}
		out[m] = NULL;
		return (out);
	}
}
