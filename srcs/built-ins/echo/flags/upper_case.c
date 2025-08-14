/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upper_case.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:58:07 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 15:14:28 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdlib.h>
#include <stdio.h>

/* Uppercase all processed_args characters */
void	handle_uppercase(char **args, char **processed_args)
{
	size_t	i;
	size_t	j;
	char	c;

	(void)args;
	i = 0;
	while (processed_args && processed_args[++i])
	{
		j = -1;
		while (processed_args[i][++j])
		{
			c = processed_args[i][j];
			if (c >= 'a' && c <= 'z')
				processed_args[i][j] = (char)(c - ('a' - 'A'));
		}
	}
}
