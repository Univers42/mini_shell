/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:44:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/17 19:55:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/* Single, process-wide history state */
t_history_state	*S(void)
{
	static t_history_state	*inst = NULL;

	if (!inst)
	{
		inst = (t_history_state *)calloc(1, sizeof(*inst));
		if (!inst)
			return NULL;
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
	static const t_history_api api = {
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

