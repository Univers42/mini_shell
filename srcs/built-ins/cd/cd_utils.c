/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:52:18 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 16:56:43 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>     // chdir, getcwd
#include <stdlib.h>     // free, NULL
#include <stdio.h>      // perror
#include "builtins.h"   // set_env_var

/* Capture oldcwd, try chdir; on failure, free oldcwd and report. */
int	cd_do_chdir(const char *target, char **oldcwd)
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
void	cd_update_pwd(char ***penv, char *oldcwd)
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
