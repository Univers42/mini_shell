/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:32:44 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 15:24:31 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <sys/types.h>

/* Need to define _POSIX_C_SOURCE for full sigaction support */
# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

/* Standard shell exit codes for signal termination */
# define EXIT_SIGINT	130		/* 128 + 2 (SIGINT) */
# define EXIT_SIGQUIT	131		/* 128 + 3 (SIGQUIT) */

/* Signal API - Independent Signal Management System */

/* Signal types for independent handling */
typedef enum e_signal_type
{
	SIG_TYPE_INT = 0,
	SIG_TYPE_QUIT,
	SIG_TYPE_COUNT
}	t_signal_type;

/* Signal operation modes */
typedef enum e_signal_mode
{
	SIG_MODE_DEFAULT,
	SIG_MODE_IGNORE,
	SIG_MODE_INTERACTIVE,
	SIG_MODE_MONITOR
}	t_signal_mode;

/* =================================================================== */
/*                    GLOBAL STATE DECLARATIONS                       */
/* =================================================================== */

/* External declarations for async-signal-safe global state */
extern volatile sig_atomic_t	g_sigint_pending;
extern volatile sig_atomic_t	g_sigquit_pending;

/* =================================================================== */
/*                    INDEPENDENT SIGINT API                          */
/* =================================================================== */

/* SIGINT Configuration */
int			sigint_set_mode(t_signal_mode mode);
int			sigint_install_handler(void (*handler)(int));
int			sigint_set_default(void);
int			sigint_set_ignore(void);

/* SIGINT State Management */
int			sigint_is_pending(void);
void		sigint_clear_pending(void);
int			sigint_get_exit_code(void);

/* SIGINT Predefined Handlers */
void		sigint_handler_interactive(int sig);
void		sigint_handler_monitor(int sig);

/* =================================================================== */
/*                    INDEPENDENT SIGQUIT API                         */
/* =================================================================== */

/* SIGQUIT Configuration */
int			sigquit_set_mode(t_signal_mode mode);
int			sigquit_install_handler(void (*handler)(int));
int			sigquit_set_default(void);
int			sigquit_set_ignore(void);

/* SIGQUIT State Management */
int			sigquit_is_pending(void);
void		sigquit_clear_pending(void);
int			sigquit_get_exit_code(void);

/* SIGQUIT Predefined Handlers */
void		sigquit_handler_monitor(int sig);

/* =================================================================== */
/*                      SIGNAL API UTILITIES                          */
/* =================================================================== */

/* Signal API Initialization */
void		signal_api_init(void);
void		signal_api_cleanup(void);

/* Generic Signal Operations */
int			signal_install(int signo, void (*handler)(int));
int			signal_set_disposition(int signo, void (*disposition)(int));

/* Signal State Management */
void		signal_clear_all_pending(void);

/* =================================================================== */
/*                   BACKWARD COMPATIBILITY LAYER                     */
/* =================================================================== */

/* Legacy API for existing minishell code */
void		setup_signals(void);
void		ignore_signals(void);
void		restore_signals(void);
void		setup_child_monitor_signals(void);
int			sigint_received(void);
int			sigquit_received(void);

/* Legacy signal_flag function */
typedef enum e_signal_action
{
	GET_SIGNAL = 0,
	SET_SIGNAL,
	RESET_SIGNAL
}	t_signal_action;

int			signal_flag(t_signal_action action, int value);

#endif