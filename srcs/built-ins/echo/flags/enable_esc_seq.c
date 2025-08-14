/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable_esc_seq.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:03:42 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 15:26:46 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/* Transform C-like escapes in processed_args[i], in place via replace buffer **/
void	handle_escapes(char **args, char **processed_args)
{
	size_t	i;

	(void)args;
	i = 1;
	while (processed_args && processed_args[i])
	{
		const char	*src = processed_args[i];
		size_t		j = 0;
		size_t		out_len = 0;
		char		*out;

		/* worst-case size == len(src) */
		out = (char *)malloc(ft_strlen(src) + 1);
		if (!out)
			return;
		while (src[j])
		{
			if (src[j] == '\\' && src[j + 1])
			{
				j++;
				if (src[j] == 'n') out[out_len++] = '\n';
				else if (src[j] == 't')
					out[out_len++] = '\t';
				else if (src[j] == 'r')
					out[out_len++] = '\r';
				else if (src[j] == 'v')
					out[out_len++] = '\v';
				else if (src[j] == 'f')
					out[out_len++] = '\f';
				else if (src[j] == 'a')
					out[out_len++] = '\a';
				else if (src[j] == 'b')
					out[out_len++] = '\b';
				else out[out_len++] = src[j];
				j++;
				continue;
			}
			out[out_len++] = src[j++];
		}
		out[out_len] = '\0';
		free(processed_args[i]);
		processed_args[i] = out;
		i++;
	}
}
