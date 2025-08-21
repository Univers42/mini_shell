/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:05:49 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 21:26:36 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

/**
 * @brief Set the signal context mask.
 *
 * Updates the context mask for signal handling.
 *
 * @param context_mask New context mask value.
 * @return int         0 on success.
 */
int	signal_set_context(int context_mask)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_context = context_mask;
	return (0);
}

/**
 * @brief Get the current signal context mask.
 *
 * Returns the current context mask used for signal handling.
 *
 * @return int Current context mask value.
 */
int	signal_get_context(void)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	return (sig_struct->g_signal_context);
}
