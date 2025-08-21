/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:00:18 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 21:26:35 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "signals.h"
#include "minishell.h"
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>

/**
 * @brief SIGINT handler for interactive mode.
 *
 * Sets SIGINT as pending and triggers readline completion.
 *
 * @param sig      Signal number (SIGINT).
 * @param info     Signal info pointer.
 * @param ucontext User context pointer.
 */
void	sigint_interactive_handler(int sig, siginfo_t *info, void *ucontext)
{
	t_core_atomic	*sig_struct;

	(void)sig;
	(void)info;
	(void)ucontext;
	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_state |= (SIG_MASK_INT << 16) | STATE_MASK_PENDING;
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return ;
	rl_done = 1;
}

/**
 * @brief SIGINT handler for monitor mode.
 *
 * Sets SIGINT as pending for monitoring purposes.
 *
 * @param sig      Signal number (SIGINT).
 * @param info     Signal info pointer.
 * @param ucontext User context pointer.
 */
void	sigint_monitor_handler(int sig, siginfo_t *info, void *ucontext)
{
	t_core_atomic	*sig_struct;

	(void)sig;
	(void)info;
	(void)ucontext;
	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_state |= (SIG_MASK_INT << 16) | STATE_MASK_PENDING;
}

/**
 * @brief SIGQUIT handler for monitor mode.
 *
 * Sets SIGQUIT as pending for monitoring purposes.
 *
 * @param sig      Signal number (SIGQUIT).
 * @param info     Signal info pointer.
 * @param ucontext User context pointer.
 */
void	sigquit_monitor_handler(int sig, siginfo_t *info, void *ucontext)
{
	t_core_atomic	*sig_struct;

	(void)sig;
	(void)info;
	(void)ucontext;
	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_state |= (SIG_MASK_QUIT << 16) | STATE_MASK_PENDING;
}
