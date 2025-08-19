/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_sigint.c                                   :+:      :+:    :+:   */
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

extern volatile sig_atomic_t	g_sigint_pending;

/*
** SIGINT Predefined Handlers
*/
void	sigint_handler_interactive(int sig)
{
	(void)sig;
	g_sigint_pending = 1;
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return ;
	rl_done = 1;
}

void	sigint_handler_monitor(int sig)
{
	(void)sig;
	g_sigint_pending = 1;
}

/*
** SIGINT Configuration
*/
int	sigint_set_mode(t_signal_mode mode)
{
	if (mode == SIG_MODE_DEFAULT)
		return (sigint_set_default());
	if (mode == SIG_MODE_IGNORE)
		return (sigint_set_ignore());
	if (mode == SIG_MODE_INTERACTIVE)
		return (sigint_install_handler(sigint_handler_interactive));
	if (mode == SIG_MODE_MONITOR)
		return (sigint_install_handler(sigint_handler_monitor));
	return (-1);
}

int	sigint_install_handler(void (*handler)(int))
{
	return (signal_install(SIGINT, handler));
}
