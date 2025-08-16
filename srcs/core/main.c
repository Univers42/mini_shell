/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:47:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/16 15:50:24 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <locale.h>
#include "libft.h"
#include "builtins.h"
#include "minishell.h"
#include "render.h"
#include "history.h"

void	ms_setup_signals(void);
/* Function to check signal flag during readline processing */
static int check_signal_flag(void)
{
	if (g_sigint_received)
	{
		/* Signal was received, tell readline to abort current line */
		rl_done = 1;
		return (0);
	}
	return (0);
}

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

	/* Setup signal handling for interactive mode */
	ms_setup_signals();

	while (run)
	{
		/* Reset signal flag before each prompt */
		g_sigint_received = 0;
		
		input = readline(build_prompt());
		
		/* Handle Ctrl+C that occurred during readline */
		if (g_sigint_received)
		{
			g_sigint_received = 0;  /* Reset the flag */
			if (input)
				free(input);
			continue;  /* Start new prompt cycle */
		}
		
		/* Handle Ctrl+D (EOF) */
		if (!input)
		{
			hs()->shutdown(); /* persist and cleanup history */
			return (ft_printf("exit\n"), 0);
		}
		if (*input)
		{
			/* Record raw user input (bash-like behavior) */
			hs()->add(input);

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
	/* normal loop end (e.g., "quit") */
	hs()->shutdown();
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

	/* make Readline aware of the current locale (UTF-8 widths, etc.) */
	setlocale(LC_ALL, "");

	ms_install_segv_handler();
	
	/* Disable readline's signal handling so we can control it ourselves */
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	
	/* Set up event hook to check for our signal flag */
	rl_event_hook = check_signal_flag;
	
	g_env = ms_dup_env(envp);
	if (!g_env)
		return (1);

	/* History singleton: constructor + load */
	{
		t_history_opts hopts = {
			.persist  = true,           /* let user switch via hs()->set_persist later if needed */
			.histfile = NULL,           /* default ~/.minishell_history or $MS_HISTORY */
			.histsize = DEFAULT_HISTSIZE /* or $MS_HISTSIZE */
		};
		if (hs()->init(&hopts, envp) == 0)
			hs()->load();
	}

	run = true;
	run_minishell(run, (t_env *)&g_env);
	return (0);
}