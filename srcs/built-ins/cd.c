/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:54:37 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 23:18:06 by syzygy           ###   ########.fr       */
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

/* Builtin cd */
int	bin_cd(char **args, int flags, t_env *env)
{
	char		***penv;
	const char	*target;
	int			print_old;
	char		*oldcwd;

	(void)flags;
	penv = (char ***)env;
	if (!penv || !*penv)
		return (ft_fprintf(2, "cd: no environment\n"), 1);
	if (args[2])
		return (ft_fprintf(2, "cd: too many arguments\n"), 1);
	target = cd_resolve_target(args, *penv, &print_old);
	if (!target)
		return (ft_fprintf(2, "cd: path not found\n"), 1);
	if (cd_do_chdir(target, &oldcwd))
		return (1);
	if (print_old)
		ft_putendl_fd((char *)target, 1);
	cd_update_pwd(penv, oldcwd);
	return (0);
}
