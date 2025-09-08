/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:35:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/22 19:09:34 by dlesieur         ###   ########.fr       */
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

void		dispatch_command(t_cmdline *cmd, t_env *env);
char		*expand_basic(const char *in);
void		print_parse_error(const char *cmd, t_parse_err err, t_env *env);
t_parse_err	parse_line(const char *line, t_cmdline *out);
void		cmdline_free(t_cmdline *cmd);

static void	clear_meta_line(void)
{
	const char	*clr_seq = "\x1b[2A\x1b[2K\x1b[2B";
	ssize_t		wr;

	if (ms()->render_mode != RENDER_FANCY)
		return ;
	wr = write(STDOUT_FILENO, clr_seq, sizeof("\x1b[2A\x1b[2K\x1b[2B") - 1);
	(void)wr;
}

static bool	handle_sigint_after_readline(char *input)
{
	if (!sigint_is_pending())
		return (false);
	sigint_clear_pending();
	ms()->last_status = sigint_get_exit_code();
	if (input)
	{
		rl_replace_line("", 0);
		free(input);
	}
	return (true);
}

static bool	process_input_line(char *input, t_env *env)
{
	char		*exp_input;
	t_cmdline	cmd;
	t_parse_err	err;

	hs()->add(input);
	exp_input = expand_basic(input);
	err = parse_line(exp_input, &cmd);
	if (err == PARSE_OK)
	{
		clear_meta_line();
		dispatch_command(&cmd, env);
	}
	else
	{
		clear_meta_line();
		if (err == PARSE_NOT_BUILTIN && ft_strcmp(exp_input, "quit") == 0)
			return (free(exp_input), cmdline_free(&cmd), false);
		if (err == PARSE_INVALID_FLAG)
			ms()->last_status = 2;
		else
			ms()->last_status = 127;
		print_parse_error(exp_input, err, env);
	}
	return (cmdline_free(&cmd), free(exp_input), true);
}

// New: run a single command string (for -c), setting last_status and returning it.
static int	run_dash_c(const char *cmdstr, t_env *env)
{
	char		*exp_input;
	t_cmdline	cmd;
	t_parse_err	err;

	if (!cmdstr || !*cmdstr)
		return (0);
	exp_input = expand_basic(cmdstr);
	err = parse_line(exp_input, &cmd);
	if (err == PARSE_OK)
	{
		dispatch_command(&cmd, env);
	}
	else if (err == PARSE_EMPTY)
	{
		ms()->last_status = 0;
	}
	else
	{
		if (err == PARSE_INVALID_FLAG)
			ms()->last_status = 2;
		else
			ms()->last_status = 127;
		print_parse_error(exp_input, err, env);
	}
	cmdline_free(&cmd);
	free(exp_input);
	return (ms()->last_status);
}

int	run_minishell(bool run, t_env *env, t_ms *app)
{
	char	*input;

	signal_api_init();
	setup_signals();

	// Proper -c handling: minishell -c "<command>"
	if (app && app->argc >= 2 && app->argv && ft_strcmp(app->argv[1], "-c") == 0)
	{
		int	status;

		if (app->argc < 3 || !app->argv[2] || !*app->argv[2])
		{
			ft_fprintf(2, "minishell: -c: requires an argument\n");
			hs()->shutdown();
			return (2);
		}
		status = run_dash_c(app->argv[2], env);
		hs()->shutdown();
		return (status);
	}

	while (run)
	{
		signal_clear_all_pending();
		input = readline("minishell > ");
		if (handle_sigint_after_readline(input))
			continue ;
		if (!input)
		{
			hs()->shutdown();
			return (ft_printf("exit\n"), 0);
		}
		if (*input)
			run = process_input_line(input, env);
		free(input);
	}
	hs()->shutdown();
	return (0);
}
