/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
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
#include "signals.h"
#include "ft_debug.h"

/* singleton storage */
static t_ms	**ms_slot(void)
{
	static t_ms	*slot = NULL;

	return (&slot);
}

t_ms	*ms(void)
{
	return (*ms_slot());
}

void	ms_install(t_ms *ptr)
{
	*ms_slot() = ptr;
}

/* Function to check signal flag during readline processing */
static int	check_signal_flag(void)
{
	if (sigint_is_pending())
	{
		rl_done = 1;
		return (0);
	}
	return (0);
}

/* very small expansion: replace $?, and leading ~ */
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
		size_t	hl;

		hl = strlen(home);
		memcpy(out + o, home, hl);
		o += hl;
		i = 1;
	}
	while (i < in_len)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			char	num[32];
			int		n;

			n = snprintf(num, sizeof(num), "%d", ms()->last_status);
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
	bins[cmd->bin_idx].builtin(cmd->argv, cmd->flags, env);
	ms()->last_status = 0;
}

static int	run_minishell(bool run, t_env *env, int argc, char **argv)
{
	t_string		input;
	char			*exp_input;
	t_cmdline		cmd;
	t_parse_err		err;

	(void)argc;
	(void)argv;
	/* Initialize the modular signal API */
	signal_api_init();
	/* Setup signal handling for interactive mode */
	setup_signals();

	while (run)
	{
		/* Clear all pending signals before each prompt */
		signal_clear_all_pending();
		
		input = readline(build_prompt());
		
		/* Handle Ctrl+C that occurred during readline */
		if (sigint_is_pending())
		{
			sigint_clear_pending();
			ms()->last_status = sigint_get_exit_code();
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
				if (ms()->render_mode == RENDER_FANCY)
				{
					const char	*clr_seq = "\x1b[2A\x1b[2K\x1b[2B";
					ssize_t		wr;

					wr = write(STDOUT_FILENO, clr_seq,
							sizeof("\x1b[2A\x1b[2K\x1b[2B") - 1);
					(void)wr;
				}
				dispatch_command(&cmd, env);
			}
			else
			{
				/* Clear metadata line also on errors so it disappears */
				if (ms()->render_mode == RENDER_FANCY)
				{
					const char	*clr_seq = "\x1b[2A\x1b[2K\x1b[2B";
					ssize_t		wr;

					wr = write(STDOUT_FILENO, clr_seq,
							sizeof("\x1b[2A\x1b[2K\x1b[2B") - 1);
					(void)wr;
				}
				if (err == PARSE_NOT_BUILTIN && ft_strcmp(exp_input, "quit") == 0)
					run = false;
				else
				{
					ms()->last_status = (err == PARSE_INVALID_FLAG) ? 2 : 127;
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
	bool	run;
	char	**clone_envp;
	t_ms	app;

	(void)argc;
	(void)argv;
	setlocale(LC_ALL, "");
	ms_install_segv_handler();
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	rl_event_hook = check_signal_flag;
	clone_envp = dup_env(envp);
	if (!clone_envp)
		return (1);
	{
		t_history_opts	hopts;

		hopts.persist = true;
		hopts.histfile = NULL;
		hopts.histsize = DEFAULT_HISTSIZE;
		if (hs()->init(&hopts, envp) == 0)
			hs()->load();
	}
	/* initialize app singleton */
	ft_bzero(&app, sizeof(app));
	app.last_status = 0;
	app.render_mode = RENDER_FANCY;
	ms_install(&app);
	run = true;
	run_minishell(run, (t_env *)&clone_envp, argc, argv);
	return (0);
}