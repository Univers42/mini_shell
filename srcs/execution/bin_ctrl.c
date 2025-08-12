/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_ctrl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:15:25 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/12 13:27:48 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_hash(char *cmd)
{
	if (!cmd || !*cmd)
		return (BUILTIN_NOT_FOUND);
	// Simple linear search for builtin name
	t_builtins *bins = access_builtins();
	for (int i = 0; bins[i].name != NULL; ++i) {
		if (strcmp(cmd, bins[i].name) == 0)
			return i;
	}
	return BUILTIN_NOT_FOUND;
}

int	is_builtin(char *cmd)
{
	return (builtin_hash(cmd) != BUILTIN_NOT_FOUND);
}