/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:35:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/23 21:39:25 by dlesieur         ###   ########.fr       */
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
#include "parser.h" // for ms_parse_tokens

void		dispatch_command(t_cmdline *cmd, t_env *env);
char		*expand_basic(const char *in);
void		print_parse_error(const char *cmd, t_parse_err err, t_env *env);
t_parse_err	parse_line(const char *line, t_cmdline *out);
void		cmdline_free(t_cmdline *cmd);

/* --- chaining helpers --- */
static int	is_chain_op(const char *tok)
{
	return (tok && (ft_strcmp(tok, "&&") == 0 || ft_strcmp(tok, "||") == 0));
}

static int	has_chain_ops(char **toks)
{
	int i = 0;

	while (toks && toks[i])
	{
		if (is_chain_op(toks[i]))
			return (1);
		i++;
	}
	return (0);
}

static int	exec_one_segment(char **toks, int start, int end, t_env *env)
{
	char		**seg_argv;
	int			len;
	int			i;
	t_cmdline	cmd;
	t_parse_err	err;

	len = end - start;
	if (len <= 0)
		return (ms()->last_status = 2, 0);
	seg_argv = (char **)ft_calloc((size_t)len + 1, sizeof(char *));
	if (!seg_argv)
		return (ms()->last_status = 1, 0);
	i = 0;
	while (i < len)
	{
		seg_argv[i] = ft_strdup(toks[start + i]);
		if (!seg_argv[i])
			return (free_tokens(seg_argv), ms()->last_status = 1, 0);
		i++;
	}
	seg_argv[len] = NULL;
	ft_bzero(&cmd, sizeof(cmd));
	cmd.argv = seg_argv;
	err = ms_parse_tokens(seg_argv, &cmd);
	if (err == PARSE_OK)
		dispatch_command(&cmd, env);
	else if (err == PARSE_NOT_BUILTIN)
	{
		char	***penv = (char ***)env;
		char	**envp = (penv && *penv) ? *penv : NULL;
		(void)exec_internal(len, seg_argv, envp);
	}
	else if (err == PARSE_EMPTY)
		ms()->last_status = 0;
	else if (err == PARSE_INVALID_FLAG)
		ms()->last_status = 2;
	cmdline_free(&cmd); /* frees seg_argv */
	return (1);
}

static int	run_chained_tokens(char **toks, t_env *env)
{
	int	ntok;
	int	i;
	int	seg_start;
	int	run_this;
	int	prev_op; /* 0 none, 1 &&, 2 || */

	ntok = count_tokens(toks);
	i = 0;
	prev_op = 0;
	while (i < ntok)
	{
		/* compute segment [seg_start, i_until_op) */
		seg_start = i;
		while (i < ntok && !is_chain_op(toks[i]))
			i++;
		/* decide execution based on previous operator and last_status */
		if (prev_op == 0)
			run_this = 1;
		else if (prev_op == 1) /* && */
			run_this = (ms()->last_status == 0);
		else /* prev_op == 2 -> || */
			run_this = (ms()->last_status != 0);
		if (run_this)
			exec_one_segment(toks, seg_start, i, env);
		/* read next operator for the NEXT segment */
		if (i < ntok && is_chain_op(toks[i]))
		{
			if (ft_strcmp(toks[i], "&&") == 0)
				prev_op = 1;
			else
				prev_op = 2;
			i++;
		}
		else
			break;
	}
	return (1);
}

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
	char		**toks;

	hs()->add(input);

	/* fast path: if the line contains && or ||, run chained execution */
	toks = lex_line(input);
	if (toks && has_chain_ops(toks))
	{
		clear_meta_line();
		run_chained_tokens(toks, env);
		free_tokens(toks);
		return (true);
	}
	free_tokens(toks);

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
		printf("PARSE EMPTY\n");
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
