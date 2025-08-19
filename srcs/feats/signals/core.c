/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include "signals.h"
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>

/* Internal pending bits (private to this TU) */
#define PEND_INT   (1u << 0)
#define PEND_QUIT  (1u << 1)

/* Private singleton: no global symbols exported */
typedef struct s_sigcore {
	volatile sig_atomic_t	state;    /* pending flags (PEND_*) */
	volatile sig_atomic_t	context;  /* CONTEXT_MASK_* */
} t_sigcore;

static inline t_sigcore *ss(void)
{
	static t_sigcore inst = { 0, CONTEXT_MASK_SHELL };
	return &inst;
}

/* Predefined handlers (use singleton) */
static void	sigint_interactive_handler(int sig)
{
	(void)sig;
	ss()->state |= PEND_INT;
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return;
	rl_done = 1;
}

static void	sigint_monitor_handler(int sig)
{
	(void)sig;
	ss()->state |= PEND_INT;
}

static void	sigquit_monitor_handler(int sig)
{
	(void)sig;
	ss()->state |= PEND_QUIT;
}

/* Core signal operations */
int	signal_configure(int signal_mask, int mode_mask, int context_mask)
{
	int	result = 0;

	(void)context_mask; /* reserved for future handler selection by context */

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
	int state = 0;

	if ((signal_mask & SIG_MASK_INT) && (ss()->state & PEND_INT))
		state |= STATE_MASK_PENDING;
	if ((signal_mask & SIG_MASK_QUIT) && (ss()->state & PEND_QUIT))
		state |= STATE_MASK_PENDING;
	return (state);
}

int	signal_set_state(int signal_mask, int state_mask)
{
	if (state_mask & STATE_MASK_PENDING)
	{
		if (signal_mask & SIG_MASK_INT)
			ss()->state |= PEND_INT;
		if (signal_mask & SIG_MASK_QUIT)
			ss()->state |= PEND_QUIT;
	}
	return (0);
}

int	signal_clear_state(int signal_mask, int state_mask)
{
	if (state_mask & STATE_MASK_PENDING)
	{
		if (signal_mask & SIG_MASK_INT)
			ss()->state &= ~PEND_INT;
		if (signal_mask & SIG_MASK_QUIT)
			ss()->state &= ~PEND_QUIT;
	}
	return (0);
}

int	signal_check_pending(int signal_mask)
{
	int pending = 0;

	if ((signal_mask & SIG_MASK_INT) && (ss()->state & PEND_INT))
		pending |= SIG_MASK_INT;
	if ((signal_mask & SIG_MASK_QUIT) && (ss()->state & PEND_QUIT))
		pending |= SIG_MASK_QUIT;
	return (pending);
}

int	signal_set_context(int context_mask)
{
	ss()->context = context_mask;
	return (0);
}

int	signal_get_context(void)
{
	return (int)ss()->context;
}

/*
** Signal API Initialization
*/
void	signal_api_init(void)
{
	ss()->state = 0;
	ss()->context = CONTEXT_MASK_SHELL;
}

void	signal_api_cleanup(void)
{
	signal_configure(SIG_MASK_ALL, MODE_MASK_DEFAULT, CONTEXT_MASK_SHELL);
	ss()->state = 0;
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
** Convenience: clear all pendings
*/
void	signal_clear_all_pending(void)
{
	ss()->state &= ~(PEND_INT | PEND_QUIT);
}
