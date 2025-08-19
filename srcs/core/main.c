/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 19:17:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/19 13:32:42 by syzygy           ###   ########.fr       */
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

void install_segv_handler(void);
void	setup_signals(void);
/* Function to check signal flag during readline processing */
static int check_signal_flag(void)
{
	if (sigint_received())
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

// change signature to receive shell env (t_env*), not argc/argv/envp
static void	print_parse_error(const char *cmd, t_parse_err err, t_env *env)
{
	if (err == PARSE_NOT_BUILTIN)
	{
		// Split the user input into tokens and run as external command
		char **toks = lex_line(cmd);
		if (toks)
		{
			int targc = count_tokens(toks);
			// extract real envp from t_env* (which is a char***)
			char ***penv = (char ***)env;
			char **envp = (penv && *penv) ? *penv : NULL;
			(void)exec_internal(targc, toks, envp);
			free_tokens(toks);
		}
		return;
	}
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

static int	run_minishell(bool run, t_env *env, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	t_string	input;
	char		*exp_input;
	t_cmdline	cmd;
	t_parse_err	err;

	/* Setup signal handling for interactive mode */
	setup_signals();

	while (run)
	{
		/* Reset signal flags before each prompt using singleton pattern */
		signal_flag(RESET_SIGNAL, 0);
		
		input = readline(build_prompt());
		
		/* Handle Ctrl+C that occurred during readline */
		if (sigint_received())
		{
			signal_flag(RESET_SIGNAL, 0); 
			g_last_status = EXIT_SIGINT; /* Set exit status for SIGINT */
			if (input)
			{
				rl_replace_line("", 0);
				free(input);
			}
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

			err = parse_line(exp_input, &cmd);
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
					// pass env (t_env*) instead of argc/argv/envp
					print_parse_error(exp_input, err, env);
				}
			}
			cmdline_free(&cmd);
			free(exp_input);
		}
		free(input);
	}
	/* normal loop end (e.g., "quit") */
	hs()->shutdown();
	return (0);
}

static char	**dup_env(char **envp)
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
	char	**clone_envp;

	/* make Readline aware of the current locale (UTF-8 widths, etc.) */
	setlocale(LC_ALL, "");

	install_segv_handler();
	
	/* Disable readline's signal handling so we can control it ourselves */
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	
	/* Set up event hook to check for our signal flag */
	rl_event_hook = check_signal_flag;
	
	clone_envp = dup_env(envp);
	if (!clone_envp)
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
	run_minishell(run, (t_env *)&clone_envp, argc, argv);
	return (0);
}