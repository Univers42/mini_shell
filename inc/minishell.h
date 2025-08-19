/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:55 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 12:36:55 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "builtins.h"
# include "signals.h"
/* Parsing state machine */
typedef enum e_parse_state
{
	ST_START = 0,
	ST_CMD,
	ST_FLAGS,
	ST_ARGS,
	ST_DONE,
	ST_ERROR
}	t_parse_state;

/* Parsing error codes */
typedef enum e_parse_err
{
	PARSE_OK = 0,
	PARSE_EMPTY,
	PARSE_NOT_BUILTIN,
	PARSE_INVALID_FLAG
}	t_parse_err;




/* Parsed command line */
typedef struct s_cmdline
{
	char		**argv;
	int			argc;
	int			flags;
	int			bin_idx;    /* index in access_builtins() or BIN_NOT_FOUND */
	t_parse_err	err;
}	t_cmdline;

/* Lexer API */
char	**ms_lex_line(const char *line);
int		ms_count_tokens(char **tokens);
void	ms_free_tokens(char **tokens);

/* Parser API */
t_parse_err	ms_parse_line(const char *line, t_cmdline *out);
void		ms_cmdline_free(t_cmdline *cmd);






int exec_internal(int argc, char **argv, char **envp);
#endif