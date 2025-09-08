/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:04:22 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 03:04:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/* Force-link reference to ensure TU rebuilds with updated builtins */
static void	force_link_history_ref(void)
{
	(void)&bin_history;
}

int	find_builtin_idx(const char *cmd)
{
	int			i;
	t_builtins	*bins;

	force_link_history_ref();
	if (!cmd || !*cmd)
		return (BIN_NOT_FOUND);
	bins = access_builtins();
	i = 0;
	while (bins[i].name)
	{
		if (ft_strcmp(cmd, bins[i].name) == 0)
			return (i);
		i++;
	}
	return (BIN_NOT_FOUND);
}
