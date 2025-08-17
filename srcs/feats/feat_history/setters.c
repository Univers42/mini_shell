/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:54:23 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 21:46:43 by dlesieur         ###   ########.fr       */
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
		custom_stifle_history(n);
	else
		custom_unstifle_history();
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

/**
 * lazy init to ensure we always record 
 * skip whitespace only
 * prevent consecutive duplicates (readline side)
 * 
*/
void	api_add(const char *line)
{
	t_history_state	*st;
	HIST_ENTRY		*last;
	const char		*p;
	int				n;

	st = S();
	if (!st || !line || !*line)
		return;
	if (!st->initialized)
	{
		if (api_init(NULL, NULL) != 0)
			return;
	}
	p = line;
	while (*p && ft_isspace((unsigned char)*p))
		p++;
	if (*p == '\0')
		return ;
	{
		n = history_length;
		if (n > 0)
		{
			last = history_get(n);
			if (last && last->line && ft_strcmp(last->line, line) == 0)
				return ;
		}
	}
	dll_push_tail_line(line);
	add_history(line);
	if (st->histsize > 0)
		custom_stifle_history(st->histsize);
}