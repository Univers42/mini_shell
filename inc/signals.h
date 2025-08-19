/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:32:44 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 12:38:02 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

/* Shell exit status codes for signals */
# define EXIT_SIGINT	130		/* Ctrl+C (SIGINT) */
# define EXIT_SIGQUIT	131		/* Ctrl+\ (SIGQUIT) */


void    ms_signal_init(void);
void    ms_signal_reset(void);
void    ms_signal_ignore(void);
void    ms_signal_default(void);
/* Signal handling for Ctrl+C (SIGINT) and Ctrl+\ (SIGQUIT) */
void		ms_setup_signals(void);
void		ms_handle_sigint_interactive(int sig);
void		ms_handle_sigint_child(int sig);
void		ms_handle_sigquit_child(int sig);
void		ms_setup_child_monitor_signals(void);
void		ms_ignore_signals(void);
void		ms_restore_signals(void);
/* Debug/Signals */
void		ms_install_segv_handler(void);
/* Signal state checking functions */
int			ms_sigint_received(void);
int			ms_sigquit_received(void);
/* Singleton pattern for signal state management */
/* Signal action codes for singleton pattern */
typedef enum e_signal_action
{
    GET_SIGNAL = 0,
	SET_SIGNAL,
	RESET_SIGNAL
}	t_signal_action;

int			signal_flag(t_signal_action action, int value);
#endif