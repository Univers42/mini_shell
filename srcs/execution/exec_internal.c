/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_internal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:07:57 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 02:38:14 by dlesieur         ###   ########.fr       */
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
#include "minishell.h"

static int	handle_execve(char *path, char **argv, char **envp)
{
	execve(path, argv, envp);
	perror("execve");
	_exit(126);
}

static int	handle_wait_status(int pid)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		ms()->last_status = 1;
		return (0);
	}
	if (WIFEXITED(status))
		ms()->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms()->last_status = 128 + WTERMSIG(status);
	else
		ms()->last_status = 1;
	return (1);
}

static int	launch_exec(char *path, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(path);
		ms()->last_status = 1;
		return (0);
	}
	if (pid == 0)
		handle_execve(path, argv, envp);
	free(path);
	return (handle_wait_status(pid));
}

int	exec_internal(int argc, char **argv, char **envp)
{
	char	*path;

	if (argc <= 0 || !argv || !argv[0])
		return (0);
	path = resolve_exec_path(argv[0], envp);
	if (!path)
	{
		ft_fprintf(2, "Unknown command: %s\n", argv[0]);
		ms()->last_status = 127;
		return (0);
	}
	return (launch_exec(path, argv, envp));
}
