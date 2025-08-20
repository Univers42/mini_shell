/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_internal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:07:57 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 22:11:51 by dlesieur         ###   ########.fr       */
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
#include "builtins.h"

// ------------------- main entry ---------------------
/** 
 * argv is already the correct vector for execve (argv[0] = cmd)
 * 
*/
int	exec_internal(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		status;
	char	*path;

	status = 0;
	if (argc <= 0 || !argv || !argv[0])
		return (0);
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
		execve(path, argv, envp);
		perror("execve");
		_exit(126);
	}
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
