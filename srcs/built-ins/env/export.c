/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:14 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 21:51:19 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdlib.h>

/*
** print_exported_vars
** ------------------
** Print all environment variables in "declare -x" format.
** Used when export is called without arguments or with -p flag.
*/
static void	print_exported_vars(char **envp)
{
	int		i;
	char	*eq_pos;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		eq_pos = ft_strchr(envp[i], '=');
		if (eq_pos)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(envp[i], 1);
			ft_putendl_fd("", 1);
		}
		i++;
	}
}

/*
** export_key_value
** ---------------
** Handle export for KEY=VALUE format
*/
static int	export_key_value(char ***penv, const char *arg, char *eq_pos)
{
	char	*key;
	char	*value;
	int		result;

	key = ft_substr(arg, 0, eq_pos - arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (free(key), 1);
	}
	value = eq_pos + 1;
	result = set_env_var(penv, key, value);
	free(key);
	return (result);
}

/*
** export_variable
** --------------
** Export a single variable. Handles both KEY=VALUE and KEY formats.
*/
static int	export_variable(char ***penv, const char *arg)
{
	char	*eq_pos;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
		return (export_key_value(penv, arg, eq_pos));
	else
	{
		if (!is_valid_identifier(arg))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd((char *)arg, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (1);
		}
		return (set_env_var(penv, arg, ""));
	}
}

/*
** count_args
** ----------
** Count the number of arguments in args array
*/
static int	count_args(char **args)
{
	int	argc;

	argc = 0;
	while (args[argc])
		argc++;
	return (argc);
}

/*
** bin_export
** ----------
** Purpose:
**   Minishell builtin implementation for `export`.
**   Exports shell variables to the environment.
**
** Behavior:
**   - No args or -p flag: Print all exported variables
**   - With args: Export each argument as a variable
**   - KEY=VALUE format: Set variable to value
**   - KEY format: Export existing variable or create empty one
**
** Params:
**   args  -> Arg vector: ["export", ...variables]
**   flags -> FLAG_P for -p option
**   env   -> Environment variables (cast to char ***)
**
** Returns:
**   0 on success, 1 on error
*/
int	bin_export(char **args, int flags, t_env *env)
{
	char	***penv;
	int		argc;
	int		i;
	int		exit_status;

	penv = (char ***)env;
	if (!penv || !*penv)
		return (1);
	argc = count_args(args);
	if (argc == 1 || (flags & FLAG_P))
	{
		print_exported_vars(*penv);
		return (0);
	}
	exit_status = 0;
	i = 1;
	while (i < argc)
	{
		if (export_variable(penv, args[i]))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
