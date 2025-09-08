/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:05:49 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/20 20:06:19 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

int	signal_set_context(int context_mask)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_context = context_mask;
	return (0);
}

int	signal_get_context(void)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	return (sig_struct->g_signal_context);
}
