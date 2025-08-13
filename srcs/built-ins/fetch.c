/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fetch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:19 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 19:45:20 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

// Fetch the value of an environment variable from envp
char *fetch_env_value(char **envp, const char *key)
{
	size_t	len;
	size_t	i;
	char	*eq;

	if (!envp || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq && (size_t)(eq - envp[i]) == len
			&& ft_strncmp(envp[i], key, len) == 0)
			return (eq + 1);
		i++;
	}
	return (NULL);
}
