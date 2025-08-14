/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:47:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 20:24:17 by syzygy           ###   ########.fr       */
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

char    *build_prompt(void);

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
	/* minimal: call the builtin. For a correct status, builtins should set g_last_status.
	   Here we zero it to indicate success by default; adapt as you wire real execution. */
	bins[cmd->bin_idx].builtin(cmd->argv, cmd->flags, env);
	g_last_status = 0;
}

static int	run_minishell(bool run, t_env *env)
{
	t_string	input;
	t_cmdline	cmd;
	t_parse_err	err;

	while (run)
	{
		input = readline(build_prompt());
		if (!input)
			return (ft_printf("exit\n"), rl_clear_history(), 0);
		if (*input)
		{
			err = ms_parse_line(input, &cmd);
			if (err == PARSE_OK)
			{
				/* Clear only the metadata (first) line; keep the input line visible. */
				if (g_render_mode == RENDER_FANCY)
				{
					/* Move up 2 lines, clear the first line, then go back down. */
					const char *clr_seq = "\x1b[2A\x1b[2K\x1b[2B";
					ssize_t wr = write(STDOUT_FILENO, clr_seq, sizeof("\x1b[2A\x1b[2K\x1b[2B") - 1);
					(void)wr;
				}
				dispatch_command(&cmd, env);
			}
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

static char	**ms_dup_env(char **envp)
{
	size_t	n;
	size_t	i;
	char	**copy;

	n = 0;
	while (envp && envp[n])
		n++;
	copy = (char **)malloc((n + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i > 0)
				free(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	bool	run;
	char	**g_env;

	ms_install_segv_handler();
	g_env = ms_dup_env(envp);
	if (!g_env)
		return (1);
	run = true;
	run_minishell(run, (t_env *)&g_env);
	return (0);
}