/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 21:07:20 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/18 21:23:21 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "render.h"

/** run external command via fork+execve() */

// ------------------- helpers ---------------------
static void	free_split(char **arr)
{
	int i;

	if (!arr) return;
	for (i = 0; arr[i]; ++i)
		free(arr[i]);
	free(arr);
}

static char **fetch_paths(char **envp)
{
	char	**split_path;
	char	*content_path;

	(void)envp;
	content_path = getenv("PATH");
	if (!content_path)
		return (NULL);
	split_path = ft_split(content_path, ':');
	if (!split_path || !*split_path)
	{
		free_split(split_path);
		return (NULL);
	}
	return (split_path);
}

inline static bool	valid_path(const char *full_path)
{
	return (access(full_path, X_OK) == 0);
}

static char	*finder_cmd(char **paths, const char *cmd)
{
	char *full_path;

	if (!paths)
		return (NULL);
	while (*paths)
	{
		full_path = ft_strnjoin(*paths, "/", cmd, NULL);
		if (!full_path)
			return (NULL);
		if (valid_path(full_path))
			return (full_path);
		free(full_path);
		paths++;
	}
	return (NULL);
}

/* resolve full path to executable:
   - if cmd has '/', try it directly,
   - else search PATH. returns malloc'ed string or NULL. */
static char *resolve_exec_path(const char *cmd, char **envp)
{
	char **paths;
	char *path;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (valid_path(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = fetch_paths(envp);
	path = finder_cmd(paths, cmd);
	free_split(paths);
	return (path);
}

// ------------------- main entry ---------------------
int exec_internal(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		status = 0;
	char	*path;

	if (argc <= 0 || !argv || !argv[0])
		return (0);

	/* argv is already the correct vector for execve (argv[0] = cmd) */
	path = resolve_exec_path(argv[0], envp);
	if (!path)
	{
		ft_fprintf(2, "Unknown command: %s\n", argv[0]);
		g_last_status = 127;
		return (0);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(path);
		g_last_status = 1;
		return (0);
	}
	if (pid == 0)
	{
		/* child: replace image */
		execve(path, argv, envp);
		perror("execve");
		_exit(126);
	}

	/* parent: wait, set status, cleanup */
	free(path);
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		g_last_status = 1;
		return (0);
	}
	if (WIFEXITED(status))
		g_last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_last_status = 128 + WTERMSIG(status);
	else
		g_last_status = 1;

	return (1);
}