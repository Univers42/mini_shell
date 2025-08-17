/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:54:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 21:44:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

int	api_init(const t_history_opts *opts, char **envp)
{
	t_history_state		*st;
	const char			*env_hist;
	const char			*env_size;
	long				v;

	using_history();
	st = S();
	(void)envp;
	if (!st)
		return (1);
	st->persist = true;
	st->histsize = DEFAULT_HISTSIZE;
	st->histfile[0] = '\0';
	env_hist = getenv("MS_HISTORY");
	env_size = getenv("MS_HISTSIZE");
	if (opts)
	{
		st->persist = opts->persist;
		if (opts->histsize != 0)
			st->histsize = opts->histsize;
		if (opts->histfile && *opts->histfile)
			expand_hist_path(opts->histfile, st->histfile, sizeof(st->histfile));
	}
	if (env_hist && *env_hist)
		expand_hist_path(env_hist, st->histfile, sizeof(st->histfile));
	if (st->histfile[0] == '\0')
		expand_hist_path("~/" DEFAULT_HISTFILE, st->histfile, sizeof(st->histfile));
	if (env_size && *env_size)
	{
		v = ft_atol(env_size);
		if (v > 0 && v <= INT_MAX)
			st->histsize = (int)v;
	}
	if (st->histsize > 0)
		custom_stifle_history(st->histsize);
	else
		custom_unstifle_history();
	st->initialized = true;
	return (0);
}

void	api_load(void)
{
	t_history_state	*st;
	HIST_ENTRY		**arr;
	int				n;
	int				i;

	st = S();
	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;
	(void)read_history(st->histfile);
	{
		arr = history_list();
		n = history_length;
		i = 0;
		while (arr && i < n)
		{
			if (arr[i] && arr[i]->line)
				dll_push_tail_line(arr[i]->line);
			i++;
		}
	}
}

void	api_save(void)
{
	t_history_state	*st;

	st = S();
	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;
	(void)write_history(st->histfile);
}
