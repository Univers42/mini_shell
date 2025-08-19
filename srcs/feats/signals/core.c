/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:30:00 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/19 17:53:24 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include "signals.h"
#include "minishell.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>

/* Global signal state storage (async-signal-safe) */
volatile sig_atomic_t	g_signal_state = 0;
volatile sig_atomic_t	g_signal_context = CONTEXT_MASK_SHELL;

/* Predefined handlers */
static void	sigint_interactive_handler(int sig)
{
	(void)sig;
	g_signal_state |= (SIG_MASK_INT << 16) | STATE_MASK_PENDING;
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return ;
	rl_done = 1;
}

static void	sigint_monitor_handler(int sig)
{
	(void)sig;
	g_signal_state |= (SIG_MASK_INT << 16) | STATE_MASK_PENDING;
}

static void	sigquit_monitor_handler(int sig)
{
	(void)sig;
	g_signal_state |= (SIG_MASK_QUIT << 16) | STATE_MASK_PENDING;
}

/* Core signal operations */
int	signal_configure(int signal_mask, int mode_mask, int context_mask)
{
	int	result = 0;

	(void)context_mask; /* Context influences handler selection */
	
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

int	signal_get_state(int signal_mask)
{
	int	state = 0;
	
	if (signal_mask & SIG_MASK_INT)
		state |= (g_signal_state & ((SIG_MASK_INT << 16) | 0xFFFF));
	if (signal_mask & SIG_MASK_QUIT)
		state |= (g_signal_state & ((SIG_MASK_QUIT << 16) | 0xFFFF));
	
	return (state);
}

int	signal_set_state(int signal_mask, int state_mask)
{
	if (signal_mask & SIG_MASK_INT)
		g_signal_state |= (SIG_MASK_INT << 16) | state_mask;
	if (signal_mask & SIG_MASK_QUIT)
		g_signal_state |= (SIG_MASK_QUIT << 16) | state_mask;
	
	return (0);
}

int	signal_clear_state(int signal_mask, int state_mask)
{
	if (signal_mask & SIG_MASK_INT)
		g_signal_state &= ~((SIG_MASK_INT << 16) | state_mask);
	if (signal_mask & SIG_MASK_QUIT)
		g_signal_state &= ~((SIG_MASK_QUIT << 16) | state_mask);
	
	return (0);
}

int	signal_check_pending(int signal_mask)
{
	int	pending = 0;
	
	if (signal_mask & SIG_MASK_INT)
		pending |= (g_signal_state & (SIG_MASK_INT << 16) & STATE_MASK_PENDING) ? SIG_MASK_INT : 0;
	if (signal_mask & SIG_MASK_QUIT)
		pending |= (g_signal_state & (SIG_MASK_QUIT << 16) & STATE_MASK_PENDING) ? SIG_MASK_QUIT : 0;
	
	return (pending);
}

int	signal_set_context(int context_mask)
{
	g_signal_context = context_mask;
	return (0);
}

int	signal_get_context(void)
{
	return (g_signal_context);
}

/*
** Signal API Initialization
*/
void	signal_api_init(t_core_atomic *sig_struct)
{
	g_signal_state = sig_struct->g_signal_state = 0;
	g_signal_context = sig_struct->g_signal_context = CONTEXT_MASK_SHELL;
}

void	signal_api_cleanup(void)
{
	signal_configure(SIG_MASK_ALL, MODE_MASK_DEFAULT, CONTEXT_MASK_SHELL);
	g_signal_state = 0;
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
	/* clear pending bit for both signals in our compact state */
	g_signal_state &= ~(((SIG_MASK_INT << 16) | STATE_MASK_PENDING));
	g_signal_state &= ~(((SIG_MASK_QUIT << 16) | STATE_MASK_PENDING));
}
