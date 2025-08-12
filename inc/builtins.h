/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:10:55 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/12 13:32:36 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "libft.h"

// here we would use bit masks
typedef enum e_builtin_flags
{
	ECHO_N = 1 << 0,
	EXPORT_P = 1 << 1,
	UNSET_V = 1 << 2,
	UNSET_F = 1 << 3,
	CD_L = 1 << 4,
	CD_P = 1 << 5,
	FLAG_COUNT = 6
}	t_builtin_flags;

typedef char	t_env;
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
	{"echo", bin_echo, ECHO_N},
	{"cd", bin_cd, CD_L | CD_P},
	{"pwd", bin_pwd, 0},
	{"export", bin_export, EXPORT_P},
	{"unset", bin_unset, UNSET_V | UNSET_F},
	{"env", bin_env, 0},
	{"exit", bin_exit, 0},
	{NULL, NULL, 0}
	};

	return (bins);
}

typedef enum e_cmd_idx
{
	BIN_ECHO = 0,
	BIN_CD,
	BIN_PWD,
	BIN_EXPORT,
	BIN_UNSET,
	BIN_ENV,
	BIN_EXIT,
	BIN_COUNT
}	t_cmd_idx;

# define BUILTIN_NOT_FOUND -1

#endif
