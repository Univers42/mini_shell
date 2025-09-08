/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:55 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/22 18:44:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "builtins.h"
# include "signals.h"
# include "parser.h"
# include "lexer.h"
# include "history.h"
# include "render.h"

/* Parsed command line */
typedef struct s_cmdline
{
	char		**argv;
	int			argc;
	int			flags;
	int			bin_idx;	// index in access_builtins() or BIN_NOT_FOUND
	t_parse_err	err;
}				t_cmdline;

typedef struct s_ms
{
	int					argc;
	char				**argv;
	int					last_status;
	t_render_mode		render_mode;
	void				*content;
	t_cmdline			*builtins;
	//t_history			api_history;
	t_parser			*parser;
	//t_lexer				*lexer;
	void				*other_feature;
}						t_ms;

/* process-wide singleton accessor (no global variables exposed) */
t_ms	*ms(void);
void	ms_install(t_ms *ptr);

int				exec_internal(int argc, char **argv, char **envp);
#endif