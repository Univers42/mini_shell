/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   git_branch.c                                       :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

char	*get_git_branch(void)
{
	char	buf[256];
	int		n;
	char	*nl;

	n = sh_capture("git branch --show-current 2>/dev/null",
			buf, sizeof(buf));
	if (n <= 0)
		return (NULL);
	nl = strchr(buf, '\n');
	if (nl)
		*nl = '\0';
	if (buf[0] == '\0')
		return (NULL);
	return (strdup(buf));
}
