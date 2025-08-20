/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:15:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:20:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

int	reset_and_alloc_array(t_history_list_state *stt,
									size_t need,
									t_hist_entry ***out)
{
	t_hist_entry	**arr;
	size_t			asz;
	int				ok;

	arr = *(stt->hist_array_ptr);
	asz = *(stt->array_size_ptr);
	if (arr)
	{
		free_hist_array(arr, asz);
		arr = NULL;
	}
	ok = alloc_hist_array(need, &arr);
	if (!ok)
	{
		*(stt->array_size_ptr) = 0;
		*out = NULL;
		return (0);
	}
	*(stt->hist_array_ptr) = arr;
	*(stt->array_size_ptr) = need;
	*out = arr;
	return (1);
}

int	alloc_hist_array(size_t need, t_hist_entry ***out)
{
	t_hist_entry	**arr;
	size_t			i;

	arr = (t_hist_entry **)malloc(need * sizeof(t_hist_entry *));
	if (!arr)
	{
		*out = NULL;
		return (0);
	}
	i = -1;
	while (++i < need)
		arr[i] = NULL;
	*out = arr;
	return (1);
}
