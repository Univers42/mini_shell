/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:07:10 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 21:26:38 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

t_core_atomic	*get_sigcore(t_core_atomic *set)
{
	static t_core_atomic	sig_struct = {0};

	if (set)
		sig_struct = *set;
	return (&sig_struct);
}

/*
** Generic Signal Operations
*/
/**
 * @brief Clear all pending signal states for SIGINT and SIGQUIT.
 *
 * Resets the pending state bits for both SIGINT and SIGQUIT in the signal core structure.
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

/**
 * @brief Check if SIGINT is currently pending.
 *
 * Returns non-zero if SIGINT is in the pending state.
 *
 * @return int Non-zero if SIGINT is pending, zero otherwise.
 */
int	sigint_is_pending(void)
{
	return (SIGNAL_IS_PENDING(SIG_MASK_INT) != 0);
}

/**
 * @brief Clear the pending state for SIGINT.
 *
 * Removes the pending flag for SIGINT in the signal state.
 */
void	sigint_clear_pending(void)
{
	signal_clear_state(SIG_MASK_INT, STATE_MASK_PENDING);
}

/**
 * @brief Get the exit code associated with SIGINT.
 *
 * Returns the exit code to be used when SIGINT is handled.
 *
 * @return int Exit code for SIGINT.
 */
int	sigint_get_exit_code(void)
{
	return (EXIT_SIGINT);
}
