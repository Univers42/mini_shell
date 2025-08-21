/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_hooks.c                                   :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "signals.h"

static int	check_signal_flag(void)
{
	if (sigint_is_pending())
	{
		rl_done = 1;
		return (0);
	}
	return (0);
}

void	setup_readline_hooks(void)
{
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	rl_event_hook = check_signal_flag;
}
