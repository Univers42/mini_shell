/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:03:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 14:17:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/* small helpers to reduce function size and improve clarity */

void	free_partial(char **out, size_t j)
{
	while (j > 0)
		free(out[--j]);
	free(out);
}

void	cleanup_history_list(void)
{
	t_history_list_state	*stt;
	t_hist_entry			**arr;
	size_t					asz;
	size_t					i;
	t_history_state			*st;

	stt = get_history_list_state();
	arr = *(stt->hist_array_ptr);
	asz = *(stt->array_size_ptr);
	if (arr)
	{
		i = 0;
		while (i < asz)
		{
			if (arr[i])
				free(arr[i]);
			i++;
		}
		free(arr);
		*(stt->hist_array_ptr) = NULL;
		*(stt->array_size_ptr) = 0;
	}
	st = access_hist_state();
	if (st)
		st->hist_length = 0;
}

void	free_hist_array(t_hist_entry **arr, size_t asz)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (i < asz)
	{
		if (arr[i])
			free(arr[i]);
		i++;
	}
	free(arr);
}
