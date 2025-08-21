/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:23:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 02:29:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "libft.h"
#include "builtins.h"
#include "minishell.h"
#include "render.h"
#include "history.h"
#include "signals.h"

char	*expand_basic(const char *in)
{
	const char	*home;
	size_t		in_len;
	size_t		extra;
	size_t		i;
	char		*out;
	size_t		o;

	home = getenv("HOME");
	in_len = ft_strlen(in);
	extra = 0;
	i = 0;
	while (i < in_len)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			extra += 16;
			i++;
		}
		i++;
	}
	if (home && in[0] == '~' && (in[1] == '\0' || in[1] == '/'))
		extra += ft_strlen(home) + 1;
	out = (char *)malloc(in_len + extra + 1);
	if (!out)
		return (ft_strdup(in));
	o = 0;
	i = 0;
	if (home && in[0] == '~' && (in[1] == '\0' || in[1] == '/'))
	{
		size_t	hl = ft_strlen(home);
		ft_memcpy(out + o, home, hl);
		o += hl;
		i = 1;
	}
	while (i < in_len)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			char	num[32];
			int		n = ft_snprintf(num, sizeof(num), "%d", ms()->last_status);

			if (n > 0)
			{
				ft_memcpy(out + o, num, (size_t)n);
				o += (size_t)n;
			}
			i++;
		}
		else
			out[o++] = in[i];
		i++;
	}
	out[o] = '\0';
	return (out);
}
