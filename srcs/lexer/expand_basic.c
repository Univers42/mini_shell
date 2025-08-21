/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_basic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:44:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 03:17:27 by dlesieur         ###   ########.fr       */
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
char	*alloc_expanded(const char *in, const char *home);
void	do_expand(const char *in, const char *home, char *out);
void	expand_body(const char *in, size_t start, char *out, size_t *po);
void	append_status(char *out, size_t *po);

int	needs_home(const char *in, const char *home)
{
	if (!home)
		return (0);
	if (in[0] == '~' && (in[1] == '\0' || in[1] == '/'))
		return (1);
	return (0);
}

size_t	expand_home(const char *in, const char *home, char *out)
{
	size_t	hl;
	size_t	o;

	o = 0;
	if (needs_home(in, home))
	{
		hl = ft_strlen(home);
		ft_memcpy(out + o, home, hl);
		o += hl;
	}
	return (o);
}

char	*expand_basic(const char *in)
{
	const char	*home;
	char		*out;

	home = getenv("HOME");
	out = alloc_expanded(in, home);
	if (!out)
		return (ft_strdup(in));
	do_expand(in, home, out);
	return (out);
}
