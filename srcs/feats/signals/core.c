/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:30:00 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/19 20:43:57 by danielm3         ###   ########.fr       */
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
//volatile sig_atomic_t	sig_struct->g_signal_state = 0;
//volatile sig_atomic_t	sig_struct->g_signal_context = CONTEXT_MASK_SHELL;

/* Predefined handlers */

t_core_atomic *access_sig_struct(t_core_atomic *set)
{
	t_core_atomic	sig_struct;

	sig_struct = (t_core_atomic){0};
	if (set)
		sig_struct = *set;
	return (&sig_struct);
}

static void	sigint_interactive_handler(int sig, siginfo_t *info, void *ucontext)
{
	t_core_atomic *sig_struct = access_sig_struct(NULL);
	(void)sig;
	(void)ucontext;
	sig_struct->g_signal_state |= (SIG_MASK_INT << 16) | STATE_MASK_PENDING;
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return ;
	rl_done = 1;
}

static void	sigint_monitor_handler(int sig, t_core_atomic *sig_struct)
{
	t_core_atomic *singleton = sg_struct();
	(void)sig;
	sig_struct->g_signal_state |= (SIG_MASK_INT << 16) | STATE_MASK_PENDING;
}

static void	sigquit_monitor_handler(int sig, t_core_atomic *sig_struct)
{
	(void)sig;
	sig_struct->g_signal_state |= (SIG_MASK_QUIT << 16) | STATE_MASK_PENDING;
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

int	signal_get_state(int signal_mask, t_core_atomic *sig_struct)
{
	int	state = 0;
	
	if (signal_mask & SIG_MASK_INT)
		state |= (sig_struct->g_signal_state & ((SIG_MASK_INT << 16) | 0xFFFF));
	if (signal_mask & SIG_MASK_QUIT)
		state |= (sig_struct->g_signal_state & ((SIG_MASK_QUIT << 16) | 0xFFFF));
	
	return (state);
}

int	signal_set_state(int signal_mask, int state_mask, t_core_atomic *sig_struct)
{
	if (signal_mask & SIG_MASK_INT)
		sig_struct->g_signal_state |= (SIG_MASK_INT << 16) | state_mask;
	if (signal_mask & SIG_MASK_QUIT)
		sig_struct->g_signal_state |= (SIG_MASK_QUIT << 16) | state_mask;
	
	return (0);
}

int	signal_clear_state(int signal_mask, int state_mask, t_core_atomic *sig_struct)
{
	if (signal_mask & SIG_MASK_INT)
		sig_struct->g_signal_state &= ~((SIG_MASK_INT << 16) | state_mask);
	if (signal_mask & SIG_MASK_QUIT)
		sig_struct->g_signal_state &= ~((SIG_MASK_QUIT << 16) | state_mask);
	
	return (0);
}

int	signal_check_pending(int signal_mask, t_core_atomic *sig_struct)
{
	int	pending = 0;
	
	if (signal_mask & SIG_MASK_INT)
		pending |= (sig_struct->g_signal_state & (SIG_MASK_INT << 16) & STATE_MASK_PENDING) ? SIG_MASK_INT : 0;
	if (signal_mask & SIG_MASK_QUIT)
		pending |= (sig_struct->g_signal_state & (SIG_MASK_QUIT << 16) & STATE_MASK_PENDING) ? SIG_MASK_QUIT : 0;
	
	return (pending);
}

int	signal_set_context(int context_mask, t_core_atomic *sig_struct)
{
	sig_struct->g_signal_context = context_mask;
	return (0);
}

int	signal_get_context(t_core_atomic *sig_struct)
{
	return (sig_struct->g_signal_context);
}

/*
** Signal API Initialization
*/
void	signal_api_init(t_core_atomic *sig_struct)
{
	sig_struct->g_signal_state = 0;
	sig_struct->g_signal_context = CONTEXT_MASK_SHELL;
}

void	signal_api_cleanup(t_core_atomic *sig_struct)
{
	signal_configure(SIG_MASK_ALL, MODE_MASK_DEFAULT, CONTEXT_MASK_SHELL);
	sig_struct->g_signal_state = 0;
}

/*
** Generic Signal Operations
*/
int	signal_install(int signo, void (*handler)(int, t_core_atomic *))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
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
void	signal_clear_all_pending(t_core_atomic *sig_struct)
{
	/* clear pending bit for both signals in our compact state */
	sig_struct->g_signal_state &= ~(((SIG_MASK_INT << 16) | STATE_MASK_PENDING));
	sig_struct->g_signal_state &= ~(((SIG_MASK_QUIT << 16) | STATE_MASK_PENDING));
}
