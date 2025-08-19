/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_sigquit_utils.c                            :+:      :+:    :+:   */
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

extern volatile sig_atomic_t	g_sigquit_pending;

int	sigquit_is_pending(void)
{
	return (g_sigquit_pending != 0);
}

void	sigquit_clear_pending(void)
{
	g_sigquit_pending = 0;
}

int	sigquit_get_exit_code(void)
{
	return (EXIT_SIGQUIT);
}
