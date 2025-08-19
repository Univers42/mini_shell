/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_legacy_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:30:00 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/19 15:30:00 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include "signals.h"
#include "minishell.h"
#include <signal.h>

int	sigquit_received(void)
{
	return (sigquit_is_pending());
}

/*
** Legacy signal_flag function for backward compatibility
*/
int	signal_flag(t_signal_action action, int value)
{
	static volatile sig_atomic_t	signal_flags = 0;
	int								prev;

	prev = (int)signal_flags;
	if (action == SET_SIGNAL)
		signal_flags = (sig_atomic_t)value;
	else if (action == RESET_SIGNAL)
		signal_flags = 0;
	return (prev);
}
