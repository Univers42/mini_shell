/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:04:22 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/20 20:05:36 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

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
