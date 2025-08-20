/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:03:10 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/20 20:11:45 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "signals.h"
#include "minishell.h"
#include <signal.h>
#include <string.h>

/* Core signal operations */
int	signal_configure(int signal_mask, int mode_mask, int context_mask)
{
	int	result;

	(void)context_mask;
	result = 0;
	if (signal_mask & SIG_MASK_INT)
	{
		if (mode_mask & MODE_MASK_DEFAULT)
			result |= signal_set_disposition(SIGINT, SIG_DFL);
		else if (mode_mask & MODE_MASK_IGNORE)
			result |= signal_set_disposition(SIGINT, SIG_IGN);
		else if (mode_mask & MODE_MASK_INTERACTIVE)
			result |= signal_install(SIGINT, sigint_interactive_handler);
		else if (mode_mask & MODE_MASK_MONITOR)
			result |= signal_install(SIGINT, sigint_monitor_handler);
	}
	if (signal_mask & SIG_MASK_QUIT)
	{
		if (mode_mask & MODE_MASK_DEFAULT)
			result |= signal_set_disposition(SIGQUIT, SIG_DFL);
		else if (mode_mask & MODE_MASK_IGNORE)
			result |= signal_set_disposition(SIGQUIT, SIG_IGN);
		else if (mode_mask & MODE_MASK_MONITOR)
			result |= signal_install(SIGQUIT, sigquit_monitor_handler);
	}
	return (result);
}

int	signal_install(int signo, void (*handler)(int, siginfo_t *, void *))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	return (sigaction(signo, &sa, NULL));
}

int	signal_set_disposition(int signo, void (*disposition)(int))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = disposition;
	return (sigaction(signo, &sa, NULL));
}
