/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 19:17:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/19 12:24:09 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <locale.h>
#include "builtins.h"
#include "minishell.h"
#include "render.h"
#include "history.h"

void	ms_setup_signals(void);

/* Function to check signal flag during readline processing */
static int check_signal_flag(void)
{
	if (ms_sigint_received())
	{
		/* Signal was received, tell readline to abort current line */
		rl_done = 1;
		return (0);
	}
	return (0);
}

char    *build_prompt(void);



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
	ms_setup_signals();

	while (run)
	{
		/* Reset signal flags before each prompt using singleton pattern */
		signal_flag(RESET_SIGNAL, 0);
		
		input = readline(build_prompt());
		
		/* Handle Ctrl+C that occurred during readline */
		if (ms_sigint_received())
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
					// pass env (t_env*) instead of argc/argv/envp
					print_parse_error(exp_input, err, env);
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
	char	**clone_env;
	t_minishell	shell;

	shell = (t_minishell){0};
	/* make Readline aware of the current locale (UTF-8 widths, etc.) */
	setlocale(LC_ALL, "");

	ms_install_segv_handler();
	
	/* Disable readline's signal handling so we can control it ourselves */
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	
	/* Set up event hook to check for our signal flag */
	/rl_event_hook = check_signal_flag;
	
	clone_env = ms_dup_env(envp);
	if (!clone_env)
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
	run_minishell(run, (t_env *)&clone_env, argc, argv);
	return (shell.last_status);
}