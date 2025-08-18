/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:55 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/18 21:12:31 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "builtins.h"

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

/* Signal action codes for singleton pattern */
typedef enum e_signal_action
{
	GET_SIGNAL = 0,
	SET_SIGNAL,
	RESET_SIGNAL
}	t_signal_action;

/* Shell exit status codes for signals */
# define EXIT_SIGINT	130		/* Ctrl+C (SIGINT) */
# define EXIT_SIGQUIT	131		/* Ctrl+\ (SIGQUIT) */

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

/* Debug/Signals */
void		ms_install_segv_handler(void);

/* Signal handling for Ctrl+C (SIGINT) and Ctrl+\ (SIGQUIT) */
void		ms_setup_signals(void);
void		ms_handle_sigint_interactive(int sig);
void		ms_handle_sigint_child(int sig);
void		ms_handle_sigquit_child(int sig);
void		ms_setup_child_monitor_signals(void);
void		ms_ignore_signals(void);
void		ms_restore_signals(void);

/* Signal state checking functions */
int			ms_sigint_received(void);
int			ms_sigquit_received(void);

/* Singleton pattern for signal state management */
int			signal_flag(t_signal_action action, int value);
int exec_internal(int argc, char **argv, char **envp);
#endif