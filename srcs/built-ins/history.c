/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 23:56:10 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 00:03:15 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "builtins.h"
#include "history.h"

/* trim-leading check for 'history' followed by end or spaces */
static int	is_history_cmd(const char *s)
{
	const char	*p;

	if (!s)
		return (0);
	p = s;
	while (*p && ft_isspace((unsigned char)*p))
		p++;
	if (p[0] != 'h' || p[1] != 'i' || p[2] != 's' || p[3] != 't'
		|| p[4] != 'o' || p[5] != 'r' || p[6] != 'y')
		return (0);
	p += 7;
	while (*p && ft_isspace((unsigned char)*p))
		p++;
	return (*p == '\0');
}

/*
 print history list, indexed starting at 1
 uses singleton hs()->dump() which returns malloc'ed NULL-terminated array
*/
int	bin_history(char **args, int flags, t_env *env)
{
	char	**arr;
	size_t	i;
	size_t	n;
	size_t	cutoff;

	(void)args;
	(void)flags;
	(void)env;
	arr = hs()->dump();
	if (!arr)
		return (0);
	n = 0;
	while (arr[n])
		n++;
	cutoff = n;
	while (cutoff > 0 && is_history_cmd(arr[cutoff - 1]))
		cutoff--;
	i = -1;
	while (++i < n)
	{
		if (i < cutoff)
			ft_printf("%5u  %s\n", (unsigned)(i + 1), arr[i]);
		free(arr[i]);
	}
	return (free(arr), 0);
}
