/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   git_status.c                                       :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

extern char				**environ;

/* run a shell command via /bin/sh -c and capture stdout to buf (NUL-terminated) */
int	sh_capture(const char *cmd, char *buf, size_t bufsz)
{
	int		pipefd[2];
	pid_t	pid;
	ssize_t	n;

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
		char	*const argv[] = { (char *)"sh", (char *)"-c",
			(char *)cmd, NULL };

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

int	get_git_status(void)
{
	char	buf[64];
	int		n;
	long	changes;
	char	*endptr;

	n = sh_capture(
			"git status --porcelain 2>/dev/null | wc -l",
			buf, sizeof(buf));
	if (n <= 0)
		return (-1);
	changes = strtol(buf, &endptr, 10);
	if (endptr == buf)
		return (-1);
	return ((int)changes);
}