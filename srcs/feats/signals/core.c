/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_core.c                                     :+:      :+:    :+:   */
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
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>

/*
** Global signal state storage (async-signal-safe)
** These globals are necessary for signal handlers which must access
** shared state across signal calls in a thread-safe manner.
*/
volatile sig_atomic_t	g_sigint_pending = 0;
volatile sig_atomic_t	g_sigquit_pending = 0;

/*
** Signal API Initialization
*/
void	signal_api_init(void)
{
	g_sigint_pending = 0;
	g_sigquit_pending = 0;
}

void	signal_api_cleanup(void)
{
	sigint_set_default();
	sigquit_set_default();
	signal_clear_all_pending();
}

/*
** Generic Signal Operations
*/
int	signal_install(int signo, void (*handler)(int))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
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

/*
** Signal State Management
*/
void	signal_clear_all_pending(void)
{
	sigint_clear_pending();
	sigquit_clear_pending();
}
