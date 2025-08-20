/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   git_status.c                                       :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

extern char				**environ;

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