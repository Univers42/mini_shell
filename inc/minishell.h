/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:55 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 16:52:27 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "builtins.h"
# include "signals.h"
# include "parser.h"
# include "lexer.h"

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
	int		last_status;
	void	*content;
}			t_ms;

int				exec_internal(int argc, char **argv, char **envp);
#endif