/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:30 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 15:57:15 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "builtins.h"

char *get_cwd(void)
{
	char *cwd;
	char *home;
	char *relative;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (strdup("~"))
	home = getenv("HOME");
	if (home && strncmp(cwd, home, ft_strlen(home)) == 0)
	{
		
	}	
}

int	bin_pwd(char **args, int flags, t_env *env)
{
	char	*cwd;

	(void)args;
	(void)flags;
	(void)env;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
