/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:14 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/24 16:42:53 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdlib.h>
#include <string.h>

/*
** print_exported_vars
** ------------------
** Print all environment variables in "declare -x" format.
** Used when export is called without arguments or with -p flag.
*/

static void	print_exported_vars(char **envp)
{
	int		i;
	int		count;
	char	**envp_copy;

	if (!envp)
		return ;
	count = 0;
	while (envp[count])
		count++;
	envp_copy = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp_copy)
		return ;
	i = 0;
	while (i < count)
	{
		envp_copy[i] = envp[i];
		i++;
	}
	envp_copy[count] = NULL;
	ft_quick_sort_str(envp_copy, 0, count - 1);
	print_sorted_env(envp_copy, count);
	free(envp_copy);
}

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
		free(key);
		return (1);
	}
	value = eq_pos + 1;
	result = set_env_var(penv, key, value);
	free(key);
	return (result);
}

static int	export_variable(char ***penv, const char *arg)
{
	char	*eq_pos;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
		return (export_key_value(penv, arg, eq_pos));
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	return (set_env_var(penv, arg, ""));
}

static int	count_args(char **args)
{
	int	argc;

	argc = 0;
	while (args[argc])
		argc++;
	return (argc);
}

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
