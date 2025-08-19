/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:18:43 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 13:49:01 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "libft.h"

// here we would use bit masks
typedef enum e_builtin_flags
{
	FLAG_N = 1 << 0, // -n
	FLAG_P = 1 << 1, // -p
	FLAG_V = 1 << 2, // -v
	FLAG_F = 1 << 3, // -f
	FLAG_L = 1 << 4, // -l
	FLAG_E = 1 << 5, // -e
	FLAG_E_DISABLE = 1 << 6,
	FLAG_U = 1 << 7
}	t_builtin_flags;

typedef void	t_env;
typedef int		(*t_builtin)(char **args, int flags, t_env *env);

/* Small helper to map a flag character to its bit */
static inline int	flag_from_char(char c)
{
	if (c == 'n')
		return (FLAG_N);
	if (c == 'p')
		return (FLAG_P);
	if (c == 'v')
		return (FLAG_V);
	if (c == 'f')
		return (FLAG_F);
	if (c == 'l')
		return (FLAG_L);
	if (c == 'e')
		return (FLAG_E);
	if (c == 'E')
		return (FLAG_E_DISABLE);
	if (c == 'u' || c == 'U')
		return (FLAG_U);
	return (0);
}

// Builtin struct
typedef struct s_builtins
{
	t_string	name;
	t_builtin	builtin;
	int			valid_flags;
}				t_builtins;

typedef enum e_cmd_idx
{
	BIN_NOT_FOUND = -1,
	BIN_ECHO,
	BIN_CD,
	BIN_PWD,
	BIN_EXPORT,
	BIN_UNSET,
	BIN_ENV,
	BIN_EXIT,
	BIN_COUNT
}	t_cmd_idx;

/* Handler signature: mutate processed_args in place */
typedef void	(*t_echo_handler)(char **args, char **processed_args);

/* Handler descriptor */
typedef struct s_echo_rule
{
	int				flag;
	int				priority;
	t_echo_handler	handler;
}	t_echo_rule;

// Builtin function declarations
int					bin_echo(char **args, int flags, t_env *env);
int					bin_cd(char **args, int flags, t_env *env);
int					bin_pwd(char **args, int flags, t_env *env);
int					bin_export(char **args, int flags, t_env *env);
int					bin_unset(char **args, int flags, t_env *env);
int					bin_env(char **args, int flags, t_env *env);
int					bin_exit(char **args, int flags, t_env *env);
int					bin_clear(char **args, int flags, t_env *env);
int					bin_history(char **args, int flags, t_env *env); /* new */
/* Env utils already provided elsewhere */
char				*fetch_env_value(char **envp, const char *key);
int					find_var_index(char **envp, const char *key, size_t klen);
char				*join_kv(const char *key, const char *val);
int					set_env_var(char ***penv, const char *key, const char *val);
void				free_env_partial(char **env, size_t count);
int					env_len(char **envp);
int					is_valid_identifier(const char *s);
int					bin_echo(char **args, int flags, t_env *env);
void				echo_output(char **processed, int flags);
const t_echo_rule	*echo_rules(void);
size_t				echo_args_start(char **args, int valid_flags);
void				handle_escapes(char **args, char **processed_args);
char				**dup_args_vector_from(char **args, size_t start);
void				free_args_vector(char **v);
void				apply_handlers(char **args, char **processed, int flags);
int					flag_bit(char ch);
int					is_flag_set(int flags, int bit);
void				handle_uppercase(char **args, char **processed_args);

// Builtins global accessor
static inline t_builtins	*access_builtins(void)
{
	static t_builtins	bins[] = {
	{"echo", bin_echo, FLAG_N | FLAG_E | FLAG_E_DISABLE | FLAG_U},
	{"cd", bin_cd, FLAG_L | FLAG_P},
	{"pwd", bin_pwd, 0},
	{"export", bin_export, FLAG_P},
	{"unset", bin_unset, FLAG_V | FLAG_F | FLAG_P},
	{"env", bin_env, 0},
	{"exit", bin_exit, 0},
	{"clear", bin_clear, 0},
	{"history", bin_history, 0},
	{NULL, NULL, 0}
	};

	return (bins);
}
#endif
