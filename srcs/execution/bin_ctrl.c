/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_ctrl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:15:25 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/20 22:01:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_hash(char *cmd)
{
	int			i;
	t_builtins	*bins;

	if (!cmd || !*cmd)
		return (BIN_NOT_FOUND);
	i = 0;
	bins = access_builtins();
	while (bins[i].name != NULL)
	{
		if (ft_strcmp(cmd, bins[i].name) == 0)
			return (i);
		i++;
	}
	return (BIN_NOT_FOUND);
}

int	is_builtin(char *cmd)
{
	return (builtin_hash(cmd) != BIN_NOT_FOUND);
}
