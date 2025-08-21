/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_context.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:34:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 02:34:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* singleton storage */
static t_ms	**ms_slot(void)
{
	static t_ms	*slot = NULL;

	return (&slot);
}

t_ms	*ms(void)
{
	return (*ms_slot());
}

void	ms_install(t_ms *ptr)
{
	*ms_slot() = ptr;
}
