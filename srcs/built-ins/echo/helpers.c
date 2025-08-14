/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:55:42 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 15:28:19 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

/* duplicate argv but exclude leading flag tokens; keep index 0 as command */
char	**dup_args_vector_from(char **args, size_t start)
{
	size_t	argc;
	size_t	outc;
	size_t	i;
	size_t	o;
	char	**out;

	argc = 0;
	while (args && args[argc])
		argc++;
	/* out: argv[0] + all args from start..end */
	outc = 1 + (start < argc ? (argc - start) : 0);
	out = (char **)malloc(sizeof(char *) * (outc + 1));
	if (!out)
		return (NULL);
	out[0] = ft_strdup(args[0]);
	if (!out[0])
		return (free(out), NULL);
	o = 1;
	i = start;
	while (i < argc)
	{
		out[o] = ft_strdup(args[i]);
		if (!out[o])
		{
			while (o > 0)
				free(out[--o]);
			free(out);
			return (NULL);
		}
		o++;
		i++;
	}
	out[outc] = NULL;
	return (out);
}

void	free_args_vector(char **v)
{
	size_t i = 0;
	if (!v) return;
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
			bit = ms_flag_bit(args[i][j]);
			if (!bit || !(valid_flags & bit))
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}
