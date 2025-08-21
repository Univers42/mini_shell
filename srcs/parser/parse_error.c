/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:24:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 02:30:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
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

void	print_parse_error(const char *cmd, t_parse_err err, t_env *env)
{
	char    **toks;
	int     targc;
	char	***penv;
	char	**envp;

	if (err == PARSE_NOT_BUILTIN)
	{
		toks = lex_line(cmd);
		if (toks)
		{
			targc = count_tokens(toks);
			penv = (char ***)env;
			envp = (penv && *penv) ? *penv : NULL;
			(void)exec_internal(targc, toks, envp);
			free_tokens(toks);
		}
		return ;
	}
	else if (err == PARSE_INVALID_FLAG)
		ft_fprintf(STDERR_FILENO, "Unknown command: %s\n", cmd);
}