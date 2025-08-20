/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:33:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:17:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

void	set_default_state(t_history_state *st)
{
	st->persist = true;
	st->histsize = DEFAULT_HISTSIZE;
	st->histfile[0] = '\0';
}

void	apply_options(t_history_state *st, const t_history_opts *opts)
{
	if (!opts)
		return ;
	st->persist = opts->persist;
	if (opts->histsize != 0)
		st->histsize = opts->histsize;
	if (opts->histfile && *opts->histfile)
		expand_hist_path(opts->histfile, st->histfile, sizeof (st->histfile));
}

void	apply_env_histfile(t_history_state *st, const char *env_hist)
{
	if (env_hist && *env_hist)
		expand_hist_path(env_hist, st->histfile, sizeof (st->histfile));
	if (st->histfile[0] == '\0')
		expand_hist_path("~/" DEFAULT_HISTFILE, st->histfile,
			sizeof (st->histfile));
}

void	apply_env_histsize(t_history_state *st, const char *env_size)
{
	long	v;

	if (env_size && *env_size)
	{
		v = ft_atol(env_size);
		if (v > 0 && v <= INT_MAX)
			st->histsize = (int)v;
	}
	if (st->histsize > 0)
		ft_stifle_history(st->histsize);
	else
		ft_unstifle_history();
}
