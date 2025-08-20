/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:07:10 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/20 20:07:50 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

/*
** Generic Signal Operations
*/
void	signal_clear_all_pending(void)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_state &= ~(((SIG_MASK_INT << 16)
				| STATE_MASK_PENDING));
	sig_struct->g_signal_state &= ~(((SIG_MASK_QUIT << 16)
				| STATE_MASK_PENDING));
}

int	sigint_is_pending(void)
{
	return (SIGNAL_IS_PENDING(SIG_MASK_INT) != 0);
}

void	sigint_clear_pending(void)
{
	signal_clear_state(SIG_MASK_INT, STATE_MASK_PENDING);
}

int	sigint_get_exit_code(void)
{
	return (EXIT_SIGINT);
}
