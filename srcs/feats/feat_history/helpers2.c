/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 16:09:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 16:09:47 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

int	api_size(void)
{
	const t_history_state	*st = S();

	if (st)
		return (st->histsize);
	return (0);
}

void	api_set_persist(bool on)
{
	t_history_state	*st;

	st = S();
	if (!st)
		return ;
	st->persist = on;
}

void	api_set_size(int n)
{
	t_history_state	*st;

	st = S();
	if (!st)
		return ;
	st->histsize = n;
	if (n > 0)
		custom_stifle_history(n);
	else
		custom_unstifle_history();
}

const char	*api_file(void)
{
	const t_history_state	*st = S();

	if (!st)
		return (NULL);
	if (st->histfile[0])
		return (st->histfile);
	return (NULL);
}