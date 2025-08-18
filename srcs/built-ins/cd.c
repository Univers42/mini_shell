/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:54:37 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/18 21:27:22 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"
#include "ft_stdlib.h"

/* Resolve target directory: HOME if no arg, OLDPWD if "-", else args[1].
   Sets *print_old when "-" was used (to print the destination). */
static const char	*cd_resolve_target(char **args, char **envp, int *print_old)
{
	const char	*arg;

	if (print_old)
		*print_old = 0;
	if (args && args[1])
		arg = args[1];
	else
		arg = NULL;
	if (!arg || !*arg)
		return (fetch_env_value(envp, "HOME"));
	if (ft_strcmp(arg, "-") == 0)
	{
		if (print_old)
			*print_old = 1;
		return (fetch_env_value(envp, "OLDPWD"));
	}
	return (arg);
}

/* Capture oldcwd, try chdir; on failure, free oldcwd and report. */
static int	cd_do_chdir(const char *target, char **oldcwd)
{
	*oldcwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
	{
		free(*oldcwd);
		*oldcwd = NULL;
		perror("cd");
		return (1);
	}
	return (0);
}

/* After chdir, compute new PWD from kernel and update env. */
static void	cd_update_pwd(char ***penv, char *oldcwd)
{
	char	*newcwd;

	newcwd = getcwd(NULL, 0);
	if (newcwd)
	{
		if (oldcwd)
			set_env_var(penv, "OLDPWD", oldcwd);
		set_env_var(penv, "PWD", newcwd);
		free(newcwd);
	}
	free(oldcwd);
}

/* Count non-empty / non-whitespace args and return first meaningful arg.
   Returns the count. first_arg is set to the pointer inside args. */
static int	count_meaningful_args(char **args, char **first_arg)
{
	int		i;
	int		count;
	const char	*p;

	if (first_arg)
		*first_arg = NULL;
	if (!args)
		return (0);
	i = 1;
	count = 0;
	while (args[i])
	{
		p = args[i];
		while (*p && ft_isspace((unsigned char)*p))
			p++;
		if (*p != '\0')
		{
			if (first_arg && !*first_arg)
				*first_arg = args[i];
			count++;
		}
		i++;
	}
	return (count);
}

/* Build temporary argv and resolve target using cd_resolve_target.
   args: original argv; envp: env array; print_old: out param. */
static const char	*prepare_target_from_args(char **args, char **envp,
											 int *print_old)
{
	char	*tmp_argv[3];
	char	*first;

	first = NULL;
	(void)count_meaningful_args(args, &first);
	tmp_argv[0] = args ? args[0] : NULL;
	tmp_argv[1] = first;
	tmp_argv[2] = NULL;
	return (cd_resolve_target(tmp_argv, envp, print_old));
}

/* Perform chdir, optionally print OLDPWD target and update PWD/OLDPWD.
   Returns 0 on success, 1 on failure. */
static int	perform_chdir_and_update(const char *target, char ***penv,
									 int print_old)
{
	char *oldcwd;

	if (cd_do_chdir(target, &oldcwd))
		return (1);
	if (print_old)
		ft_putendl_fd((char *)target, 1);
	cd_update_pwd(penv, oldcwd);
	return (0);
}

/* Builtin cd */
int	bin_cd(char **args, int flags, t_env *env)
{
	char		***penv;
	const char	*target;
	int			print_old;
	int			meaningful;

	(void)flags;
	penv = (char ***)env;
	if (!penv || !*penv)
		return (ft_fprintf(2, "cd: no environment\n"), 1);
	meaningful = count_meaningful_args(args, NULL);
	if (meaningful > 1)
		return (ft_fprintf(2, "cd: too many arguments\n"), 1);
	target = prepare_target_from_args(args, *penv, &print_old);
	if (!target)
		return (ft_fprintf(2, "cd: path not found\n"), 1);
	return (perform_chdir_and_update(target, penv, print_old));
}
