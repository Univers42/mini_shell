/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:35:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:11:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/* Single, process-wide history state */
t_history_state	*access_hist_state(void)
{
	static t_history_state	*inst = NULL;

	if (!inst)
	{
		inst = (t_history_state *)calloc(1, sizeof(*inst));
		if (!inst)
			return (NULL);
		inst->list = ft_dll_create();
		if (!inst->list)
			return (free(inst), NULL);
		inst->persist = true;
		inst->histsize = DEFAULT_HISTSIZE;
		inst->histfile[0] = '\0';
		inst->initialized = false;
	}
	return (inst);
}

/* VTable export */
const t_history_api	*hs(void)
{
	static const t_history_api	api = {
		.init = api_init,
		.load = api_load,
		.add = api_add,
		.save = api_save,
		.dump = api_dump,
		.shutdown = api_shutdown,
		.file = api_file,
		.size = api_size,
		.set_persist = api_set_persist,
		.set_size = api_set_size
	};

	return (&api);
}

t_history_list_state	*get_history_list_state(void)
{
	static t_hist_entry			**hist_array = NULL;
	static size_t				array_size = 0;
	static t_history_list_state	state = {&hist_array, &array_size};

	return (&state);
}

/* --- api_init --- */
int	api_init(const t_history_opts *opts, char **envp)
{
	t_history_state	*st;
	const char		*env_hist;
	const char		*env_size;

	(void)envp;
	st = access_hist_state();
	if (!st)
		return (1);
	set_default_state(st);
	env_hist = getenv("MS_HISTORY");
	env_size = getenv("MS_HISTSIZE");
	apply_options(st, opts);
	apply_env_histfile(st, env_hist);
	apply_env_histsize(st, env_size);
	st->initialized = true;
	return (0);
}

void	api_shutdown(void)
{
	t_history_state	*st;

	st = access_hist_state();
	if (!st)
		return ;
	api_save();
	dll_clear_all();
	cleanup_history_list();
	rl_clear_history();
	if (st->list)
		ft_dll_destroy(st->list);
	ft_memset(st, 0, sizeof(*st));
}
