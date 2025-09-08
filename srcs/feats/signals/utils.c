/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:07:10 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/20 22:47:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

t_core_atomic	*get_sigcore(t_core_atomic *set)
{
	static t_core_atomic	sig_struct = {0};

	if (set)
		sig_struct = *set;
	return (&sig_struct);
}

/*
** Generic Signal Operations
*/
void	signal_clear_all_pending(void)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_state &= ~(((SIG_MASK_INT << 16)
				| STATE_MASK_PENDING));
	sig_struct->g_signal_state &= ~(((SIG_MASK_QUIT << 16)
				| STATE_MASK_PENDING));
}

int	sigint_is_pending(void)
{
	return (SIGNAL_IS_PENDING(SIG_MASK_INT) != 0);
}

void	sigint_clear_pending(void)
{
	signal_clear_state(SIG_MASK_INT, STATE_MASK_PENDING);
}

int	sigint_get_exit_code(void)
{
	return (EXIT_SIGINT);
}
