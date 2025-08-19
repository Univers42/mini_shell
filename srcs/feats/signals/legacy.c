/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_legacy.c                                   :+:      :+:    :+:   */
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

/*
** Legacy API for existing minishell code
*/
void	setup_signals(void)
{
	sigint_set_mode(SIG_MODE_INTERACTIVE);
	sigquit_set_mode(SIG_MODE_IGNORE);
}

void	ignore_signals(void)
{
	sigint_set_mode(SIG_MODE_IGNORE);
	sigquit_set_mode(SIG_MODE_IGNORE);
}

void	restore_signals(void)
{
	sigint_set_mode(SIG_MODE_DEFAULT);
	sigquit_set_mode(SIG_MODE_DEFAULT);
}

void	setup_child_monitor_signals(void)
{
	sigint_set_mode(SIG_MODE_MONITOR);
	sigquit_set_mode(SIG_MODE_MONITOR);
}

int	sigint_received(void)
{
	return (sigint_is_pending());
}
