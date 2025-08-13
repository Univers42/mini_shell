/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:01:21 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/13 11:27:52 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static char	*extract_env_value(const char *name, char **envp)
{
	size_t	len;
	size_t	i;
	char	*eq;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq && (size_t)(eq - envp[i]) == len
			&& ft_strncmp(envp[i], name, len) == 0)
			return (eq + 1);
		i++;
	}
	return (NULL);
}

int	bin_pwd(char **args, int flags, t_env *env)
{
	char	*pwd;
	char	*cwd;

	(void)args;
	(void)flags;
	pwd = extract_env_value("PWD", (char **)env);
	if (pwd)
	{
		ft_putendl_fd(pwd, 1);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
