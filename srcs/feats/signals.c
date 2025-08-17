/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 00:00:00 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/17 17:52:21 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "minishell.h"

/*
** Singleton flag for SIGINT state (async-signal-safe storage).
** Access only through signal_flag(). Uses sig_atomic_t for safety.
*/
int	signal_flag(t_signal_action action, int value)
{
	static volatile sig_atomic_t	sigint_flag = 0;
	int								prev;

	prev = (int)sigint_flag;
	if (action == SET_SIGNAL)
		sigint_flag = (sig_atomic_t)value;
	else if (action == RESET_SIGNAL)
		sigint_flag = 0;
	return (prev);
}

/*
** SIGINT handler (interactive prompt): just mark and newline.
** No readline calls here (not async-signal-safe).
*/
void	ms_handle_sigint_interactive(int sig)
{
	(void)sig;
	signal_flag(SET_SIGNAL, 1);
	/* show a clean break of the current line */
	/* (void)write(STDOUT_FILENO, "^C", 2); */
}

/*
** Helper to install one handler/disp via sigaction with SA_RESTART.
*/
static void	ms_install_sa(int signo, void (*handler)(int))
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
static void	ms_install_disp(int signo, void (*disp)(int))
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
void	ms_setup_signals(void)
{
	ms_install_sa(SIGINT, ms_handle_sigint_interactive);
	ms_install_disp(SIGQUIT, SIG_IGN);
}

/*
** Parent ignores while a foreground child runs (child will get default).
** Call before waiting the child; restore afterwards.
*/
void	ms_ignore_signals(void)
{
	ms_install_disp(SIGINT, SIG_IGN);
	ms_install_disp(SIGQUIT, SIG_IGN);
}

/*
** Restore defaults (use in child just before execve, or parent after wait).
*/
void	ms_restore_signals(void)
{
	ms_install_disp(SIGINT, SIG_DFL);
	ms_install_disp(SIGQUIT, SIG_DFL);
}
