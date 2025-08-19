/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_sigint_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:30:00 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/19 15:30:00 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include "signals.h"
#include "minishell.h"
#include <signal.h>

extern volatile sig_atomic_t	g_sigint_pending;

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
	return (g_sigint_pending != 0);
}

void	sigint_clear_pending(void)
{
	g_sigint_pending = 0;
}

int	sigint_get_exit_code(void)
{
	return (EXIT_SIGINT);
}
