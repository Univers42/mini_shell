/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 16:09:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/20 15:17:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * @return set ? size of curr file : 0
 * */
int	api_size(void)
{
	const t_history_state	*st = access_hist_state();

	if (st)
		return (st->histsize);
	return (0);
}

/**
 * sets the maximum number of history entries allowed (the history limit)..
 * It updats the `histsize` fied and may call `ft_stifle_history` or 
 * `ft_unstfile_history` to enforce thew new limmit
 */
void	api_set_size(int n)
{
	t_history_state	*st;

	st = access_hist_state();
	if (!st)
		return ;
	st->histsize = n;
	if (n > 0)
		ft_stifle_history(n);
	else
		ft_unstifle_history();
}

/**
 * updates the currennt count of histories entries (hist_length) in
 * the state after any change (add, remove,, stifle, unstiflle,..)
 * it does not change the limit, only recor how many entries are present
 */
void	update_history_length(void)
{
	t_history_state	*st;

	st = access_hist_state();
	if (st && st->list)
		st->hist_length = (int)st->list->size;
	else if (st)
		st->hist_length = 0;
}

/**
 * Force persistence to ON
 */
void	api_set_persist(bool on)
{
	t_history_state	*st;

	st = access_hist_state();
	if (!st)
		return ;
	st->persist = on;
}

/**
 * @brief returns the path to the current history file
 * being used by the shell
 * @return the path of file if set : NULL
 */
const char	*api_file(void)
{
	const t_history_state	*st = access_hist_state();

	if (!st)
		return (NULL);
	if (st->histfile[0])
		return (st->histfile);
	return (NULL);
}
