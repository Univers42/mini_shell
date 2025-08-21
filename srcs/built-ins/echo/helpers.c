/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:55:42 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/21 14:05:51 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

// Allocates the output vector and duplicates the command name
static char	**alloc_and_copy_cmd(char **args, size_t outc)
{
	char	**out;

	out = (char **)malloc(sizeof(char *) * (outc + 1));
	if (!out)
		return (NULL);
	out[0] = ft_strdup(args[0]);
	if (!out[0])
	{
		free(out);
		return (NULL);
	}
	return (out);
}

// Duplicates arguments from start index, returns 0 on success, -1 on failure
static int	dup_args_range(char **out, char **args, size_t start, size_t argc)
{
	size_t	o;
	size_t	i;

	o = 1;
	i = start;
	while (i < argc)
	{
		out[o] = ft_strdup(args[i]);
		if (!out[o])
			return (-1);
		o++;
		i++;
	}
	return (0);
}

// Main function, now split and simplified
char	**dup_args_vector_from(char **args, size_t start)
{
	size_t	argc;
	size_t	outc;
	char	**out;

	argc = 0;
	while (args && args[argc])
		argc++;
	if (start < argc)
		outc = 1 + (argc - start);
	else
		outc = 1;
	out = alloc_and_copy_cmd(args, outc);
	if (!out)
		return (NULL);
	if (dup_args_range(out, args, start, argc) < 0)
	{
		free_args_vector(out);
		return (NULL);
	}
	out[outc] = NULL;
	return (out);
}

void	free_args_vector(char **v)
{
	size_t	i;

	if (!v)
		return ;
	i = 0;
	while (v[i])
		free(v[i++]);
	free(v);
}

/* Compute index of first non-flag argument (skip leading -[neEuU] groups) */
size_t	echo_args_start(char **args, int valid_flags)
{
	size_t	i;
	size_t	j;
	int		bit;

	i = 1;
	while (args && args[i] && args[i][0] == '-' && args[i][1] != '\0')
	{
		j = 1;
		while (args[i][j])
		{
			bit = flag_bit(args[i][j]);
			if (!bit || !(valid_flags & bit))
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}
