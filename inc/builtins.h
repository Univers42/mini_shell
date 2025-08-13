/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:10:55 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 10:06:45 by syzygy           ###   ########.fr       */
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
	FLAG_L = 1 << 4  // -l
}	t_builtin_flags;

/* Small helper to map a flag character to its bit */
static inline int	flag_from_char(char c)
{
	if (c == 'n') return FLAG_N;
	if (c == 'p') return FLAG_P;
	if (c == 'v') return FLAG_V;
	if (c == 'f') return FLAG_F;
	if (c == 'l') return FLAG_L;
	return 0;
}

typedef void	t_env;
typedef int		(*t_builtin)(char **args, int flags, t_env *env);

// Builtin struct
typedef struct s_builtins
{
	t_string	name;
	t_builtin	builtin;
	int			valid_flags;
}				t_builtins;

// Builtin function declarations
int	bin_echo(char **args, int flags, t_env *env);
int	bin_cd(char **args, int flags, t_env *env);
int	bin_pwd(char **args, int flags, t_env *env);
int	bin_export(char **args, int flags, t_env *env);
int	bin_unset(char **args, int flags, t_env *env);
int	bin_env(char **args, int flags, t_env *env);
int	bin_exit(char **args, int flags, t_env *env);

// Builtins global accessor
static inline t_builtins	*access_builtins(void)
{
	static t_builtins	bins[] = {
	{"echo", bin_echo, FLAG_N},
	{"cd", bin_cd, FLAG_L | FLAG_P},
	{"pwd", bin_pwd, 0},
	{"export", bin_export, FLAG_P},
	{"unset", bin_unset, FLAG_V | FLAG_F | FLAG_P},
	{"env", bin_env, 0},
	{"exit", bin_exit, 0},
	{NULL, NULL, 0}
	};

	return (bins);
}

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

#endif
