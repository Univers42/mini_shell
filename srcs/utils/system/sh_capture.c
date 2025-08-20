/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_capture.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 23:04:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 23:07:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

/* run a shell command via /bin/sh -c and capture stdout to buf (NUL-terminated) */
int sh_capture(const char *cmd, char *buf, size_t bufsz)
{
	int		pipefd[2];
	pid_t	pid;
	ssize_t	n;
	char	*const argv[] = {
	(char *)"sh",
	(char *)"-c",
	(char *)cmd,
	NULL
	};

	if (!buf || bufsz == 0 || pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execve("/bin/sh", argv, environ);
		_exit(EXIT_FAILURE);
	}
	close(pipefd[1]);
	n = read(pipefd[0], buf, bufsz - 1);
	close(pipefd[0]);
	waitpid(pid, NULL, 0);
	if (n < 0)
		return (-1);
	buf[n] = '\0';
	return ((int)n);
}
