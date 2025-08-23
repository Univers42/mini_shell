/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:24 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/21 00:15:46 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include <stdlib.h>
#include <stdio.h>

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

static int	set_env(char ***penv, char **envp, char *entry)
{
	size_t	n;

	n = -1;
	while (envp[++n])
		;
	if (!ft_realloc((void **)penv, sizeof(char *), n + 1, n + 2))
		return (free(entry), 0);
	envp = *penv;
	envp[n] = entry;
	envp[n + 1] = NULL;
	return (1);
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
	if (!set_env(penv, envp, entry))
		return (ft_fprintf(STDERR_FILENO,
				"set_env_var: failed to set %s=%s\n",
				key, val), 1);
	return (0);
}

/*
** Counts the number of strings in a NULL-terminated env array.
** Used to size allocations and loops safely.
*/
int	env_len(char **envp)
{
	int	len;

	len = 0;
	while (envp[len])
		len++;
	return (len);
}
