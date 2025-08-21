/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
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

static void	clear_meta_line(void)
{
	const char	*clr_seq = "\x1b[2A\x1b[2K\x1b[2B";
	ssize_t		wr;

	if (ms()->render_mode != RENDER_FANCY)
		return ;
	wr = write(STDOUT_FILENO, clr_seq, sizeof("\x1b[2A\x1b[2K\x1b[2B") - 1);
	(void)wr;
}

static char	*expand_basic(const char *in)
{
	const char	*home;
	size_t		in_len;
	size_t		extra;
	size_t		i;
	char		*out;
	size_t		o;

	home = getenv("HOME");
	in_len = strlen(in);
	extra = 0;
	i = 0;
	while (i < in_len)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			extra += 16;
			i++;
		}
		i++;
	}
	if (home && in[0] == '~' && (in[1] == '\0' || in[1] == '/'))
		extra += strlen(home) + 1;
	out = (char *)malloc(in_len + extra + 1);
	if (!out)
		return (ft_strdup(in));
	o = 0;
	i = 0;
	if (home && in[0] == '~' && (in[1] == '\0' || in[1] == '/'))
	{
		size_t	hl = strlen(home);
		memcpy(out + o, home, hl);
		o += hl;
		i = 1;
	}
	while (i < in_len)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			char	num[32];
			int		n = snprintf(num, sizeof(num), "%d", ms()->last_status);

			if (n > 0)
			{
				memcpy(out + o, num, (size_t)n);
				o += (size_t)n;
			}
			i++;
		}
		else
			out[o++] = in[i];
		i++;
	}
	out[o] = '\0';
	return (out);
}

static void	print_parse_error(const char *cmd, t_parse_err err, t_env *env)
{
	if (err == PARSE_NOT_BUILTIN)
	{
		char	**toks = lex_line(cmd);

		if (toks)
		{
			int		targc = count_tokens(toks);
			char	***penv = (char ***)env;
			char	**envp = (penv && *penv) ? *penv : NULL;

			(void)exec_internal(targc, toks, envp);
			free_tokens(toks);
		}
		return ;
	}
	else if (err == PARSE_INVALID_FLAG)
		fprintf(stderr, "Unknown command: %s\n", cmd);
}

static void	dispatch_command(t_cmdline *cmd, t_env *env)
{
	t_builtins	*bins;

	bins = access_builtins();
	bins[cmd->bin_idx].builtin(cmd->argv, cmd->flags, env);
	ms()->last_status = 0;
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
		ms()->last_status = (err == PARSE_INVALID_FLAG) ? 2 : 127;
		print_parse_error(exp_input, err, env);
	}
	cmdline_free(&cmd);
	free(exp_input);
	return (true);
}

int	run_minishell(bool run, t_env *env, int argc, char **argv)
{
	char	*input;

	(void)argc;
	(void)argv;
	signal_api_init();
	setup_signals();
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
