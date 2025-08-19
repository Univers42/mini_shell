/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigint_utils.c                                     :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "signals.h"
#include <signal.h>

int	sigint_set_default(void)
{
	return (signal_set_disposition(SIGINT, SIG_DFL));
}

int	sigint_set_ignore(void)
{
	return (signal_set_disposition(SIGINT, SIG_IGN));
}

int	sigint_is_pending(void)
{
	return (SIGNAL_IS_PENDING(SIG_MASK_INT) != 0);
}

void	sigint_clear_pending(void)
{
	(void)signal_clear_state(SIG_MASK_INT, STATE_MASK_PENDING);
}

int	sigint_get_exit_code(void)
{
	return (EXIT_SIGINT);
}
