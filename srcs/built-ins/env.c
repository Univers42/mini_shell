/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:41:39 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/13 20:11:25 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/**
The find_var_index function searches for the index
of an environment variable in the envp array. 
@brief envp	the environment variable array
@param key the key of the environmenet variables
@param  klen the size of the key
@return	the index of the variable in the envp array
*/
int	find_var_index(char **envp, const char *key, size_t klen)
{
	int		i;
	char	*eq;

	if (!envp || !key)
		return (-1);
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq && (size_t)(eq - envp[i]) == klen
			&& ft_strncmp(envp[i], key, klen) == 0)
			return (i);
		i++;
	}
	return (-1);
}

// Build "KEY=VALUE" using ft_strjoin (two-step, free intermediate)
char	*join_kv(const char *key, const char *val)
{
	char	*kv_eq;
	char	*entry;

	kv_eq = ft_strjoin(key, "=");
	if (!kv_eq)
		return (NULL);
	entry = ft_strjoin(kv_eq, val);
	free(kv_eq);
	return (entry);
}

/*
@brief Replace or append KEY=VALUE, growing env with ft_realloc when needed.
@param penv address of env array (char***)
*/
int	set_env_var(char ***penv, const char *key, const char *val)
{
	char	**envp;
	char	*entry;
	size_t	klen;
	size_t	n;
	int		idx;

	if (!penv || !*penv || !key || !val)
		return (1);
	envp = *penv;
	klen = ft_strlen(key);
	entry = join_kv(key, val);
	if (!entry)
		return (1);
	idx = find_var_index(envp, key, klen);
	if (idx >= 0)
	{
		free(envp[idx]);
		envp[idx] = entry;
		return (0);
	}
	n = 0;
	while (envp[n])
		n++;
	if (!ft_realloc((void **)penv, sizeof(char *), n + 1, n + 2))
		return (free(entry), 1);
	envp = *penv;
	envp[n] = entry;
	envp[n + 1] = NULL;
	return (0);
}

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

static char	**dup_env(char **envp)
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

static int	builtin_env(char **argv, char **envp)
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
	return (builtin_env(args, *(char ***)env));
}
