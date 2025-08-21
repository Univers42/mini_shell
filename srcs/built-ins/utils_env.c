/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:24 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/21 02:18:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include <stdlib.h>
#include <stdio.h>

void	update_pwd_vars(char **envp, const char *old_pwd);


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

/*
** is_valid_identifier
** ------------------
** Check if a string is a valid shell variable identifier.
** Must start with letter/underscore, followed by letters/digits/underscores.
** Used by unset/export to validate variable names.
** Returns 1 if valid, 0 otherwise.
*/
int	is_valid_identifier(const char *s)
{
	size_t	i;

	if (!s || !s[0] || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	**dup_env(char **envp)
{
	size_t	n;
	size_t	i;
	char	**copy;

	n = 0;
	while (envp && envp[n])
		n++;
	copy = (char **)malloc((n + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i > 0)
				free(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[n] = NULL;
	return (copy);
}
