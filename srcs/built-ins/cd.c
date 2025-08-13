/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:30:16 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 20:30:18 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"
#include "ft_stdlib.h"

/**
	@brief After chdir, compute new PWD from kernel to be canonical
	@param penv pointer to change to access and modify penv
	@param oldcwd pointer to the old current working directory
	@param newcwd pointer to the new current working directory
	@return current working directory
*/
static void compute_new_pwd(char ***penv, char *oldcwd, char *newcwd)
{
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

/**
Capture current pwd before moving
and then set the oldcwd
*/
static int	move_path(char *oldcwd, const char *target)
{
	oldcwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
		return (free(oldcwd), perror("cd"), 1);
	return (0);
}

/**
	@param args the arguments passed to the cd command
	@param flags the flags passed to the cd command
	@param env the environment variable structure (treated as char***)
	@return the exit status
*/
int	bin_cd(char **args, int flags, t_env *env)
{
	char		***penv;
	char		**mut_env;
	const char	*arg;
	const char	*target;
	char		*oldcwd;
	char		*newcwd;

	(void)flags;
	penv = (char ***)env;
	mut_env = *penv;
	if (args && args[1])
		arg = args[1];
	else
		arg = NULL;
	if (!arg || !*arg)
		target = fetch_env_value(mut_env, "HOME");
	else if (ft_strcmp(arg, "-") == 0)
	{
		target = fetch_env_value(mut_env, "OLDPWD");
		if (target)
			ft_putendl_fd((char *)target, 1);
	}
	else
		target = arg;
	if (!target)
		return (ft_fprintf(2, "cd: path not found\n"), 1);
	oldcwd = NULL;
	if (move_path(oldcwd, target))
		return (ft_fprintf(STDERR_FILENO, "cd: %s: No such file or directory\n", target), 1);
	compute_new_pwd(penv, oldcwd, newcwd);
	return (0);
}