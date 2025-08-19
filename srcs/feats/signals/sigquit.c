/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_sigquit.c                                  :+:      :+:    :+:   */
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

extern volatile sig_atomic_t	g_sigquit_pending;

/*
** SIGQUIT Predefined Handlers
*/
void	sigquit_handler_monitor(int sig)
{
	(void)sig;
	g_sigquit_pending = 1;
}

/*
** SIGQUIT Configuration
*/
int	sigquit_set_mode(t_signal_mode mode)
{
	if (mode == SIG_MODE_DEFAULT)
		return (sigquit_set_default());
	if (mode == SIG_MODE_IGNORE)
		return (sigquit_set_ignore());
	if (mode == SIG_MODE_MONITOR)
		return (sigquit_install_handler(sigquit_handler_monitor));
	if (mode == SIG_MODE_INTERACTIVE)
		return (sigquit_set_ignore());
	return (-1);
}

int	sigquit_install_handler(void (*handler)(int))
{
	return (signal_install(SIGQUIT, handler));
}

int	sigquit_set_default(void)
{
	return (signal_set_disposition(SIGQUIT, SIG_DFL));
}

int	sigquit_set_ignore(void)
{
	return (signal_set_disposition(SIGQUIT, SIG_IGN));
}
