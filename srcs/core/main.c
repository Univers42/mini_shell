/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:47:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/12 14:27:49 by syzygy           ###   ########.fr       */
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

static void	print_parse_error(const char *cmd, t_parse_err err)
{
	if (err == PARSE_NOT_BUILTIN)
		printf("Unknown command: %s\n", cmd);
	else if (err == PARSE_INVALID_FLAG)
		printf("Unknown command: %s\n", cmd);
}

static void	dispatch_command(t_cmdline *cmd, t_env *env)
{
	t_builtins	*bins;

	bins = access_builtins();
	bins[cmd->bin_idx].builtin(cmd->argv, cmd->flags, env);
}

static int	run_minishell(bool run, t_env *env)
{
	t_string	input;
	t_cmdline	cmd;
	t_parse_err	err;

	while (run)
	{
		input = readline("minishell> ");
		if (!input)
			return (printf("exit\n"), rl_clear_history(), 0);
		if (*input)
		{
			err = ms_parse_line(input, &cmd);
			if (err == PARSE_OK)
				dispatch_command(&cmd, env);
			else if (err == PARSE_NOT_BUILTIN && ft_strcmp(input, "quit") == 0)
				run = false;
			else
				print_parse_error(input, err);
			ms_cmdline_free(&cmd);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	bool	run;
	t_env	env;

	env.envp = envp;
	run = true;
	run_minishell(run, &env);
	return (0);
}
