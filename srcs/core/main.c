/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:47:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 20:50:35 by syzygy           ###   ########.fr       */
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

/* very small expansion: replace all "$?" with last status, and a leading "~" or "~/"
   with $HOME. Returns malloc'ed string (caller frees). */
static char *expand_basic(const char *in)
{
	const char *home = getenv("HOME");
	size_t in_len = strlen(in);
	size_t extra = 0;
	size_t i;

	/* size pass */
	for (i = 0; i < in_len; ++i)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			extra += 16;
			++i;
		}
	}
	if (home && (in[0] == '~') && (in[1] == '\0' || in[1] == '/'))
		extra += strlen(home) + 1; /* replace '~' */

	char *out = (char *)malloc(in_len + extra + 1);
	if (!out)
		return ft_strdup(in);

	size_t o = 0;
	i = 0;

	/* handle leading ~ */
	if (home && in[0] == '~' && (in[1] == '\0' || in[1] == '/'))
	{
		size_t hl = strlen(home);
		memcpy(out + o, home, hl);
		o += hl;
		i = 1; /* skip '~' */
	}
	for (; i < in_len; ++i)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			char num[32];
			int n = snprintf(num, sizeof(num), "%d", g_last_status);
			if (n > 0)
			{
				memcpy(out + o, num, (size_t)n);
				o += (size_t)n;
			}
			++i; /* skip '?' */
		}
		else
		{
			out[o++] = in[i];
		}
	}
	out[o] = '\0';
	return out;
}

static void	print_parse_error(const char *cmd, t_parse_err err)
{
	if (err == PARSE_NOT_BUILTIN)
		fprintf(stderr, "Unknown command: %s\n", cmd);
	else if (err == PARSE_INVALID_FLAG)
		fprintf(stderr, "Unknown command: %s\n", cmd);
}

static void	dispatch_command(t_cmdline *cmd, t_env *env)
{
	t_builtins	*bins;

	bins = access_builtins();
	/* Call the builtin. If it doesn't set status, we'll assume success. */
	bins[cmd->bin_idx].builtin(cmd->argv, cmd->flags, env);
	/* Default to success for builtins unless they changed it explicitly. */
	g_last_status = 0;
}

static int	run_minishell(bool run, t_env *env)
{
	t_string	input;
	char		*exp_input;
	t_cmdline	cmd;
	t_parse_err	err;

	while (run)
	{
		input = readline(build_prompt());
		if (!input)
			return (ft_printf("exit\n"), rl_clear_history(), 0);
		if (*input)
		{
			/* light expansion before parsing */
			exp_input = expand_basic(input);

			err = ms_parse_line(exp_input, &cmd);
			if (err == PARSE_OK)
			{
				/* Clear only the metadata (first) line; keep the input line visible. */
				if (g_render_mode == RENDER_FANCY)
				{
					const char *clr_seq = "\x1b[2A\x1b[2K\x1b[2B";
					ssize_t wr = write(STDOUT_FILENO, clr_seq, sizeof("\x1b[2A\x1b[2K\x1b[2B") - 1);
					(void)wr;
				}
				dispatch_command(&cmd, env);
			}
			else
			{
				/* Clear metadata line also on errors so it disappears */
				if (g_render_mode == RENDER_FANCY)
				{
					const char *clr_seq = "\x1b[2A\x1b[2K\x1b[2B";
					ssize_t wr = write(STDOUT_FILENO, clr_seq, sizeof("\x1b[2A\x1b[2K\x1b[2B") - 1);
					(void)wr;
				}
				if (err == PARSE_NOT_BUILTIN && ft_strcmp(exp_input, "quit") == 0)
					run = false;
				else
				{
					/* Standard shell-like statuses */
					g_last_status = (err == PARSE_INVALID_FLAG) ? 2 : 127;
					print_parse_error(exp_input, err);
				}
			}
			ms_cmdline_free(&cmd);
			free(exp_input);
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