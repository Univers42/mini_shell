/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:04:22 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 21:26:39 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

/**
 * @brief Get the current state of selected signals.
 *
 * Returns the state bits for SIGINT and/or SIGQUIT as specified by the mask.
 *
 * @param signal_mask Bitmask selecting signals (SIG_MASK_INT, SIG_MASK_QUIT).
 * @return int        State bits for selected signals.
 */
int	signal_get_state(int signal_mask)
{
	t_core_atomic	*sig_struct;
	int				state;

	state = 0;
	sig_struct = get_sigcore(NULL);
	if (signal_mask & SIG_MASK_INT)
		state |= (sig_struct->g_signal_state & ((SIG_MASK_INT << 16) | 0xFFFF));
	if (signal_mask & SIG_MASK_QUIT)
		state |= (sig_struct->g_signal_state
				& ((SIG_MASK_QUIT << 16) | 0xFFFF));
	return (state);
}

/**
 * @brief Set state bits for selected signals.
 *
 * Sets the specified state bits for SIGINT and/or SIGQUIT.
 *
 * @param signal_mask Bitmask selecting signals.
 * @param state_mask  State bits to set.
 * @return int        0 on success.
 */
int	signal_set_state(int signal_mask, int state_mask)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	if (signal_mask & SIG_MASK_INT)
		sig_struct->g_signal_state |= (SIG_MASK_INT << 16) | state_mask;
	if (signal_mask & SIG_MASK_QUIT)
		sig_struct->g_signal_state |= (SIG_MASK_QUIT << 16) | state_mask;
	return (0);
}

/**
 * @brief Clear state bits for selected signals.
 *
 * Clears the specified state bits for SIGINT and/or SIGQUIT.
 *
 * @param signal_mask Bitmask selecting signals.
 * @param state_mask  State bits to clear.
 * @return int        0 on success.
 */
int	signal_clear_state(int signal_mask, int state_mask)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	if (signal_mask & SIG_MASK_INT)
		sig_struct->g_signal_state &= ~((SIG_MASK_INT << 16) | state_mask);
	if (signal_mask & SIG_MASK_QUIT)
		sig_struct->g_signal_state &= ~((SIG_MASK_QUIT << 16) | state_mask);
	return (0);
}

/**
 * @brief Check if any selected signals are pending.
 *
 * Returns non-zero if any of the selected signals are in the pending state.
 *
 * @param signal_mask Bitmask selecting signals.
 * @return int        Non-zero if any are pending, zero otherwise.
 */
int	signal_check_pending(int signal_mask)
{
	t_core_atomic	*sig_struct;
	int				pending;

	sig_struct = get_sigcore(NULL);
	pending = 0;
	if (signal_mask & SIG_MASK_INT)
	{
		if (sig_struct->g_signal_state & (SIG_MASK_INT << 16)
			& STATE_MASK_PENDING)
			pending |= SIG_MASK_INT;
	}
	if (signal_mask & SIG_MASK_QUIT)
	{
		if (sig_struct->g_signal_state & (SIG_MASK_QUIT << 16)
			& STATE_MASK_PENDING)
			pending |= SIG_MASK_QUIT;
	}
	return (pending);
}
