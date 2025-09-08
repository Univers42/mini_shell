/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable_esc_seq.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:03:42 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/21 14:17:35 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static char	handle_single_escape(const char *src, size_t *j)
{
	char	c;

	c = src[*j];
	if (c == 'n')
		return ('\n');
	if (c == 't')
		return ('\t');
	if (c == 'r')
		return ('\r');
	if (c == 'v')
		return ('\v');
	if (c == 'f')
		return ('\f');
	if (c == 'a')
		return ('\a');
	if (c == 'b')
		return ('\b');
	return (c);
}

static void	process_escape(char *src, char **dst)
{
	size_t	j;
	size_t	out_len;
	char	*out;

	j = 0;
	out_len = 0;
	out = (char *)malloc(ft_strlen(src) + 1);
	if (!out)
		return ;
	while (src[j])
	{
		if (src[j] == '\\' && src[j + 1])
		{
			j++;
			out[out_len++] = handle_single_escape(src, &j);
			j++;
		}
		else
		{
			out[out_len++] = src[j++];
		}
	}
	out[out_len] = '\0';
	*dst = out;
}

void	handle_escapes(char **args, char **processed_args)
{
	size_t	i;
	char	*out;

	(void)args;
	i = 1;
	while (processed_args && processed_args[i])
	{
		out = NULL;
		process_escape(processed_args[i], &out);
		if (out)
		{
			free(processed_args[i]);
			processed_args[i] = out;
		}
		i++;
	}
}
