/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <sys/types.h>

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

# define EXIT_SIGINT	130
# define EXIT_SIGQUIT	131

/* =================================================================== */
/*                        BIT MASK DEFINITIONS                        */
/* =================================================================== */

/* Signal type masks */
# define SIG_MASK_INT		(1 << 0)	/* SIGINT */
# define SIG_MASK_QUIT		(1 << 1)	/* SIGQUIT */
# define SIG_MASK_ALL		(SIG_MASK_INT | SIG_MASK_QUIT)

/* Signal mode masks */
# define MODE_MASK_DEFAULT		(1 << 0)
# define MODE_MASK_IGNORE		(1 << 1)
# define MODE_MASK_INTERACTIVE	(1 << 2)
# define MODE_MASK_MONITOR		(1 << 3)

/* Signal state masks */
# define STATE_MASK_PENDING		(1 << 0)
# define STATE_MASK_PROCESSED	(1 << 1)
# define STATE_MASK_BLOCKED		(1 << 2)

/* Context masks */
# define CONTEXT_MASK_SHELL		(1 << 0)
# define CONTEXT_MASK_CHILD		(1 << 1)
# define CONTEXT_MASK_READLINE	(1 << 2)
# define CONTEXT_MASK_EXEC		(1 << 3)

/* =================================================================== */
/*                     PATTERN-BASED PROTOTYPES                       */
/* =================================================================== */

typedef void	(*t_signal_handler)(int sig);
typedef int		(*t_signal_config_fn)(int signal_mask, int mode_mask);
typedef int		(*t_signal_state_fn)(int signal_mask, int state_mask);
typedef int		(*t_signal_context_fn)(int signal_mask, int context_mask);

typedef struct s_signal_op
{
	int					signal_mask;
	int					mode_mask;
	int					state_mask;
	int					context_mask;
	t_signal_handler	handler;
	t_signal_config_fn	config_fn;
	t_signal_state_fn	state_fn;
	t_signal_context_fn	context_fn;
}	t_signal_op;

typedef int sig_atomic_t;
typedef struct s_core_atomic
{
	volatile sig_atomic_t 	g_signal_state;
	volatile sig_atomic_t 	g_signal_context;
}							t_core_atomic;
/* =================================================================== */
/*                      DYNAMIC SIGNAL API                            */
/* =================================================================== */

int		signal_configure(int signal_mask, int mode_mask, int context_mask);
int		signal_get_state(int signal_mask);
int		signal_set_state(int signal_mask, int state_mask);
int		signal_clear_state(int signal_mask, int state_mask);
int		signal_check_pending(int signal_mask);

int		signal_install_dynamic(int signal_mask, int mode_mask, int context_mask);

int		signal_set_context(int context_mask);
int		signal_get_context(void);
int		signal_context_configure(int context_mask);

int		signal_execute_op(const t_signal_op *op);
int		signal_batch_configure(const t_signal_op *ops, int count);

/* =================================================================== */
/*                    EFFICIENT UTILITY MACROS                        */
/* =================================================================== */

# define SIGNAL_IS_PENDING(sig_mask)	(signal_check_pending(sig_mask))
# define SIGNAL_SET_MODE(sig_mask, mode_mask) \
	signal_configure(sig_mask, mode_mask, signal_get_context())

# define SIGNAL_ENTER_SHELL()		signal_set_context(CONTEXT_MASK_SHELL)
# define SIGNAL_ENTER_CHILD()		signal_set_context(CONTEXT_MASK_CHILD)
# define SIGNAL_ENTER_READLINE()	signal_set_context(CONTEXT_MASK_READLINE)
# define SIGNAL_ENTER_EXEC()		signal_set_context(CONTEXT_MASK_EXEC)

# define SIGNAL_CONFIGURE_ALL(mode_mask) \
	signal_configure(SIG_MASK_ALL, mode_mask, signal_get_context())

# define SIGNAL_CLEAR_ALL_PENDING() \
	signal_clear_state(SIG_MASK_ALL, STATE_MASK_PENDING)

/* =================================================================== */
/*                   BACKWARD COMPATIBILITY LAYER                     */
/* =================================================================== */

# define setup_signals() \
	signal_configure(SIG_MASK_INT, MODE_MASK_INTERACTIVE, CONTEXT_MASK_SHELL); \
	signal_configure(SIG_MASK_QUIT, MODE_MASK_IGNORE, CONTEXT_MASK_SHELL)

# define ignore_signals() \
	SIGNAL_CONFIGURE_ALL(MODE_MASK_IGNORE)

# define restore_signals() \
	SIGNAL_CONFIGURE_ALL(MODE_MASK_DEFAULT)

# define setup_child_monitor_signals() \
	signal_set_context(CONTEXT_MASK_CHILD); \
	SIGNAL_CONFIGURE_ALL(MODE_MASK_MONITOR)

# define sigint_received() \
	SIGNAL_IS_PENDING(SIG_MASK_INT)

# define sigquit_received() \
	SIGNAL_IS_PENDING(SIG_MASK_QUIT)

/* Legacy signal_flag function */
typedef enum e_signal_action
{
	GET_SIGNAL = 0,
	SET_SIGNAL,
	RESET_SIGNAL
}	t_signal_action;

int		signal_flag(t_signal_action action, int value);

/* Legacy-style helpers kept as wrappers so existing code compiles */
int		sigint_set_default(void);
int		sigint_set_ignore(void);
int		sigint_is_pending(void);
void	sigint_clear_pending(void);
int		sigint_get_exit_code(void);

int		sigquit_set_default(void);
int		sigquit_set_ignore(void);
int		sigquit_is_pending(void);
void	sigquit_clear_pending(void);
int		sigquit_get_exit_code(void);

/* =================================================================== */
/*                      SIGNAL API UTILITIES                          */
/* =================================================================== */

int		signal_mask_to_signo(int signal_mask);
int		signo_to_signal_mask(int signo);
int		signal_get_exit_code(int signal_mask);

/* Predefined op configs (optional) */
extern const t_signal_op	g_signal_ops_shell[];
extern const t_signal_op	g_signal_ops_child[];
extern const t_signal_op	g_signal_ops_readline[];
extern const t_signal_op	g_signal_ops_exec[];

/* Low-level install helpers */
int				signal_install(int signo, void (*handler)(int, siginfo_t *, void *));
int				signal_set_disposition(int signo, void (*disposition)(int));
void			signal_api_init(void);
void			signal_api_cleanup(void);
void			signal_clear_all_pending(void);

#endif
