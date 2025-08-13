/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 12:02:08 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 13:09:18 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

// Helper: get value of key in envp
static char *get_env_value(char **envp, const char *key)
{
	size_t	len;
	size_t	i;
	char	*eq;

	len = ft_strlen(key);
	i = 0;
	while (envp && envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq && (size_t)(eq - envp[i]) == len
			&& ft_strncmp(envp[i], key, len) == 0)
			return (eq + 1);
		i++;
	}
	return (NULL);
}

// Helper: update envp in-place (not robust, just for demo)
static void update_env_var(char **envp, const char *key, const char *value)
{
	size_t	len;
	size_t	i;
	char	*eq;

	len = ft_strlen(key);
	i = 0;
	while (envp && envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq && (size_t)(eq - envp[i]) == len
			&& ft_strncmp(envp[i], key, len) == 0)
		{
			size_t old_len = ft_strlen(envp[i]);
			size_t new_len = len + 1 + ft_strlen(value);
			if (new_len <= old_len) {
				ft_strlcpy(envp[i] + len + 1, value, old_len - len);
			}
			// Otherwise, do nothing (or print a warning)
			return;
		}
		i++;
	}
	// Not found: not implemented (would need to realloc envp)
}

// Only expand ~, otherwise return a copy of path
static char *resolve_path(const char *path, char **envp)
{
	char *home;
	char *result;
	size_t len_home, len_path;

	if (path[0] == '~')
	{
		home = get_env_value(envp, "HOME");
		if (!home)
			return (ft_strdup(path));
		if (path[1] == '\0')
			return (ft_strdup(home));
		else if (path[1] == '/')
		{
			len_home = ft_strlen(home);
			len_path = ft_strlen(path + 1);
			result = malloc(len_home + len_path + 1);
			if (!result)
				return (NULL);
			ft_strlcpy(result, home, len_home + 1);
			ft_strlcat(result, path + 1, len_home + len_path + 1);
			return (result);
		}
	}
	// If path is relative, prepend "./" if not already present
	if (path[0] != '/' && !(path[0] == '.' && (path[1] == '/' || path[1] == '\0')))
	{
		size_t len_path = ft_strlen(path);
		result = malloc(2 + len_path + 1);
		if (!result)
			return (NULL);
		ft_strlcpy(result, "./", 3);
		ft_strlcat(result, path, 2 + len_path + 1);
		return (result);
	}
	return (ft_strdup(path));
}

// Update PWD and OLDPWD variables
void update_pwd_vars(char **envp, const char *old_pwd)
{
	char *cwd;

	if (old_pwd)
		update_env_var(envp, "OLDPWD", old_pwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		update_env_var(envp, "PWD", cwd);
		free(cwd);
	}
	// else: fallback, do not update PWD
}

// Main cd function
static int my_cd(const char *path, char **envp)
{
	const char *target_path;
	char *resolved_path;
	char *old_pwd;
	char *cwd;

	if (!path || path[0] == '\0')
		target_path = get_env_value(envp, "HOME");
	else if (ft_strcmp(path, "-") == 0)
	{
		target_path = get_env_value(envp, "OLDPWD");
		if (target_path)
			ft_printf("%s\n", target_path);
	}
	else
		target_path = path;

	if (!target_path)
	{
		ft_fprintf(STDERR_FILENO, "cd: path not found\n");
		return (1);
	}
	resolved_path = resolve_path(target_path, envp);
	if (!resolved_path)
	{
		ft_fprintf(STDERR_FILENO, "cd: memory allocation failed\n");
		return (1);
	}
	old_pwd = get_env_value(envp, "PWD");
	if (chdir(resolved_path) != 0)
	{
		perror("cd");
		free(resolved_path);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		update_env_var(envp, "OLDPWD", old_pwd ? old_pwd : "");
		update_env_var(envp, "PWD", cwd);
		free(cwd);
	}
	free(resolved_path);
	return (0);
}

// Builtin cd function
int bin_cd(char **args, int flags, t_env *env)
{
	(void)flags;
	return my_cd(args[1], (char **)env);
}