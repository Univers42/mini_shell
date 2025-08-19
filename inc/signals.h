/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:32:44 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 14:16:00 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

/* Shell exit status codes for signals */
# define EXIT_SIGINT	130		/* Ctrl+C (SIGINT) */
# define EXIT_SIGQUIT	131		/* Ctrl+\ (SIGQUIT) */

void		signal_init(void);
void		signal_reset(void);
void		signal_ignore(void);
void		signal_default(void);
/* Signal handling for Ctrl+C (SIGINT) and Ctrl+\ (SIGQUIT) */
void		setup_signals(void);
void		handle_sigint_interactive(int sig);
void		handle_sigint_child(int sig);
void		handle_sigquit_child(int sig);
void		setup_child_monitor_signals(void);
void		ignore_signals(void);
void		restore_signals(void);
/* Debug/Signals */
void		install_segv_handler(void);
/* Signal state checking functions */
int			sigint_received(void);
int			sigquit_received(void);

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