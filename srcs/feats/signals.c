/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:00:00 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 14:37:14 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <string.h>
#include <signals.h>

/*
** Singleton flags for signal state (async-signal-safe storage).
** Access only through signal_flag(). Uses sig_atomic_t for safety.
** Bit 0: SIGINT flag, Bit 1: SIGQUIT flag
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

/*
** SIGINT handler (interactive prompt): just mark and newline.
** No readline calls here (not async-signal-safe).
*/
void	handle_sigint_interactive(int sig)
{
	(void)sig;
	signal_flag(SET_SIGNAL, signal_flag(GET_SIGNAL, 0) | 1);
	/* show a clean break of the current line */
	/* (void)write(STDOUT_FILENO, "^C", 2); */
}

/*
** SIGQUIT handler (for child process monitoring in parent).
** Sets bit 1 in the signal flags to indicate SIGQUIT received.
*/
void	handle_sigquit_child(int sig)
{
	(void)sig;
	signal_flag(SET_SIGNAL, signal_flag(GET_SIGNAL, 0) | 2);
}

/*
** Helper to install one handler/disp via sigaction with SA_RESTART.
*/
static void	install_sa(int signo, void (*handler)(int))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	sigaction(signo, &sa, NULL);
}

/*
** Helper to set a disposition (SIG_IGN or SIG_DFL).
*/
static void	install_disp(int signo, void (*disp)(int))
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = disp;
	sigaction(signo, &sa, NULL);
}

/*
** Interactive shell: INT handled -> newline+flag, QUIT ignored.
*/
void	setup_signals(void)
{
	install_sa(SIGINT, handle_sigint_interactive);
	install_disp(SIGQUIT, SIG_IGN);
}

/*
** Parent ignores while a foreground child runs (child will get default).
** Call before waiting the child; restore afterwards.
*/
void	ignore_signals(void)
{
	install_disp(SIGINT, SIG_IGN);
	install_disp(SIGQUIT, SIG_IGN);
}

/*
** Restore defaults (use in child just before execve, or parent after wait).
*/
void	restore_signals(void)
{
	install_disp(SIGINT, SIG_DFL);
	install_disp(SIGQUIT, SIG_DFL);
}

/*
** Check if SIGINT was received (bit 0).
*/
int	sigint_received(void)
{
	return (signal_flag(GET_SIGNAL, 0) & 1);
}

/*
** Check if SIGQUIT was received (bit 1).
*/
int	sigquit_received(void)
{
	return (signal_flag(GET_SIGNAL, 0) & 2);
}

/*
** Setup signals for monitoring child process execution.
** Parent catches both SIGINT and SIGQUIT to detect when child should terminate.
*/
void	setup_child_monitor_signals(void)
{
	install_sa(SIGINT, handle_sigint_interactive);
	install_sa(SIGQUIT, handle_sigquit_child);
}
