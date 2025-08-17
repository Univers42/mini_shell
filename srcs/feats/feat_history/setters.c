/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:54:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 16:59:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

int	api_size(void)
{
	t_history_state *st = S();

	st = S();
	if (st)
		return (st->histsize);
	return (0);
}

void	api_set_persist(bool on)
{
	t_history_state	*st;

	st = S();
	if (!st)
		return ;
	st->persist = on;
}

void	api_set_size(int n)
{
	t_history_state *st;

	st = S();
	if (!st)
		return;
	st->histsize = n;
	if (n > 0)
		stifle_history(n);
	else
		unstifle_history();
}

const char	*api_file(void)
{
	t_history_state *st;

	st = S();
	if (!st)
		return (NULL);
	if (st->histfile[0])
		return (st->histfile);
	return (NULL);
}

void	api_add(const char *line)
{
	t_history_state	*st;
	const	char *p;

	st = S();
	if (!st || !st->initialized || !line || !*line)
		return;
	{
		p = line;
		while (*p && ft_isspace((unsigned char)*p))
			p++;
		if (*p == '\0')
			return ;
	}
	dll_push_tail_line(line);
	add_history(line);
	if (st->histsize > 0)
		stifle_history(st->histsize);
}