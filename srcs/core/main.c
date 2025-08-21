/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:34:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 02:34:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <readline/readline.h>
#include "libft.h"
#include "minishell.h"
#include "history.h"
#include "signals.h"
#include "render.h"
#include "core_utils.h"

static void	init_history(char **envp)
{
	t_history_opts	hopts;

	hopts.persist = true;
	hopts.histfile = NULL;
	hopts.histsize = DEFAULT_HISTSIZE;
	if (hs()->init(&hopts, envp) == 0)
		hs()->load();
}

int	main(int argc, char **argv, char **envp)
{
	bool	run;
	char	**clone_envp;
	t_ms	app;

	(void)argc;
	(void)argv;
	ms_install_segv_handler();
	setup_readline_hooks();
	clone_envp = dup_env(envp);
	if (!clone_envp)
		return (1);
	init_history(envp);
	ft_bzero(&app, sizeof(app));
	app.last_status = 0;
	app.render_mode = RENDER_FANCY;
	ms_install(&app);
	run = true;
	return (run_minishell(run, (t_env *)&clone_envp, argc, argv));
}
