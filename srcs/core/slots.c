/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slots.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:21:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 02:27:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "libft.h"
#include "builtins.h"
#include "minishell.h"
#include "render.h"
#include "history.h"
#include "signals.h"

/**
 * 
 * In this archive we will find all the ports for each API
 * For now we already have
 * - history
 * - builtins
 */

void	dispatch_command(t_cmdline *cmd, t_env *env)
{
	t_builtins	*bins;

	bins = access_builtins();
	bins[cmd->bin_idx].builtin(cmd->argv, cmd->flags, env);
	ms()->last_status = 0;
}

