/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_capture.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:19:43 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 21:44:08 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

/* environ is provided by libc */
extern char	**environ;

static int	open_pipe(int fds[2])
{
	if (pipe(fds) < 0)
		return (-1);
	return (0);
}

static pid_t	spawn_shell(const char *cmd, int fds[2])
{
	pid_t	pid;
	char	*argv[4];

	pid = fork();
	if (pid != 0)
		return (pid);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
		_exit(127);
	close(fds[0]);
	close(fds[1]);
	argv[0] = "/bin/sh";
	argv[1] = "-c";
	argv[2] = (char *)cmd;
	argv[3] = NULL;
	execve("/bin/sh", argv, environ);
	_exit(127);
}

static ssize_t	read_to_buf(int fd, char *buf, size_t bufsz)
{
	ssize_t	total;
	ssize_t	n;

	total = 0;
	if (bufsz == 0)
		return (0);
	while ((size_t)total + 1 < bufsz)
	{
		n = read(fd, buf + total, bufsz - (size_t)total - 1);
		if (n <= 0)
			break ;
		total += n;
	}
	buf[total] = '\0';
	return (total);
}

static int	wait_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
		return (-1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (-1);
}

int	sh_capture(const char *cmd, char *buf, size_t bufsz)
{
	int		fds[2];
	pid_t	pid;
	ssize_t	nread;

	if (!cmd || !buf)
		return (-1);
	if (open_pipe(fds) < 0)
		return (-1);
	pid = spawn_shell(cmd, fds);
	if (pid < 0)
	{
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	close(fds[1]);
	nread = read_to_buf(fds[0], buf, bufsz);
	close(fds[0]);
	(void)wait_child(pid);
	return ((int)nread);
}
