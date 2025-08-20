/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:06:39 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/20 20:15:32 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"

/*
** Signal API Initialization
*/
void	signal_api_init(void)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	sig_struct->g_signal_state = 0;
	sig_struct->g_signal_context = CONTEXT_MASK_SHELL;
}

void	signal_api_cleanup(void)
{
	t_core_atomic	*sig_struct;

	sig_struct = get_sigcore(NULL);
	signal_configure(SIG_MASK_ALL, MODE_MASK_DEFAULT, CONTEXT_MASK_SHELL);
	sig_struct->g_signal_state = 0;
}
