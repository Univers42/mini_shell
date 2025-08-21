/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:41:52 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 03:16:14 by dlesieur         ###   ########.fr       */
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

void	copy_until_qmark(const char *in, size_t *pi, char *out, size_t *po);
int		needs_home(const char *in, const char *home);
size_t	expand_home(const char *in, const char *home, char *out);
size_t	calc_extra(const char *in, const char *home);
void	copy_until_qmark(const char *in, size_t *pi, char *out, size_t *po);

void	append_status(char *out, size_t *po)
{
	char	num[32];
	int		n;

	n = ft_snprintf(num, sizeof(num), "%d", ms()->last_status);
	if (n > 0)
	{
		ft_memcpy(out + *po, num, (size_t)n);
		*po += (size_t)n;
	}
}

void	expand_body(const char *in, size_t start, char *out, size_t *po)
{
	size_t	i;

	i = start;
	while (in[i])
	{
		copy_until_qmark(in, &i, out, po);
		if (in[i] == '$' && in[i + 1] == '?')
		{
			append_status(out, po);
			i += 2;
		}
	}
}

void	do_expand(const char *in, const char *home, char *out)
{
	size_t	o;
	size_t	start;

	o = 0;
	if (needs_home(in, home))
		o = expand_home(in, home, out);
	if ((needs_home(in, home)))
		start = 1;
	else
		start = 0;
	expand_body(in, start, out, &o);
	out[o] = '\0';
}

char	*alloc_expanded(const char *in, const char *home)
{
	size_t	in_len;
	size_t	extra;

	in_len = ft_strlen(in);
	extra = calc_extra(in, home);
	return ((char *)malloc(in_len + extra + 1));
}

size_t	count_qmark_extra(const char *in)
{
	size_t	i;
	size_t	extra;

	i = 0;
	extra = 0;
	while (in[i])
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			extra += 16;
			i++;
		}
		i++;
	}
	return (extra);
}
