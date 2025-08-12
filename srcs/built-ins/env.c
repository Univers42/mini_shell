/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:41:39 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/12 21:42:02 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	free_env_partial(char **env, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(env[i]);
		i++;
	}
	free(env);
}

char	**dup_env(char **envp)
{
	char	**env_copy;
	size_t	n;
	size_t	i;

	n = 0;
	while (envp[n])
		n++;
	env_copy = malloc((n + 1) * sizeof(*env_copy));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		env_copy[i] = ft_strdup(envp[i]);
		if (!env_copy[i])
		{
			free_env_partial(env_copy, i);
			return (NULL);
		}
		i++;
	}
	env_copy[n] = NULL;
	return (env_copy);
}

static int	print_env(char **env)
{
	size_t	i;
	int		fail;

	if (!env)
		return (1);
	i = 0;
	fail = 0;
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			if (write(1, env[i], ft_strlen(env[i])) < 0
				|| write(1, "\n", 1) < 0)
				fail = 1;
		}
		i++;
	}
	return (fail);
}

int	builtin_env(char **argv, char **envp)
{
	char	**envp_copy;
	size_t	count;
	int		status;

	if (argv && argv[1])
	{
		if (write(2, "env: No such file or directory\n", 31) < 0)
			return (1);
		return (127);
	}
	envp_copy = dup_env(envp);
	if (!envp_copy)
		return (1);
	count = 0;
	while (envp_copy[count])
		count++;
	status = print_env(envp_copy);
	free_env_partial(envp_copy, count);
	return (status);
}

int	bin_env(char **args, int flags, t_env *env)
{
	(void)flags;
	return (builtin_env(args, (char **)env));
}
