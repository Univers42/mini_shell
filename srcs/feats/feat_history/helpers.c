/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:51:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/19 14:04:37 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

const char	*expand_hist_path(const char *name, char *out, size_t outsz)
{
	const char	*home;
	size_t		len;

	if (!name || !out || outsz == 0)
		return NULL;
	home = getenv("HOME");
	if (name[0] == '~' && home && *home)
	{
		if (name[1] == '/' || name[1] == '\0')
		{
			if (ft_snprintf(out, outsz, "%s/%s", home, name + 1) <= 0)
				return (NULL);
			return (out);
		}
	}
	len = ft_strclen(name, '\0');
	if (len > outsz - 1)
		len = outsz - 1;
	ft_strlcpy(out, name, outsz);
	out[len] = '\0';
	return (out);
}

void    dll_push_tail_line(const char *line)
{
	t_history_state	*st;
	char			*dup;
	char			*last;
	char			*old;

	st = S();
	if (!st || !st->list || !line || !*line)
		return ;
	if (st->list->size > 0)
	{
		last = (char *)ft_dll_back(st->list);
		if (last && ft_strcmp(last, line) == 0)
			return ;
	}
	dup = ft_strdup(line);
	if (!dup)
		return ;
	if (!ft_dll_push_back(st->list, dup))
		free(dup);
	else if (st->histsize > 0 && st->list->size > (size_t)st->histsize)
	{
		old = (char *)ft_dll_pop_front(st->list);
		free(old);
	}
	update_history_length(); /* updates g_custom_history_length */
}

void    dll_clear_all(void)
{
	t_history_state		*st;
	char				*s;

	st = S();
	if (!st || !st->list)
		return;
	while (!ft_dll_is_empty(st->list))
	{
		s = (char *)ft_dll_pop_front(st->list);
		free(s);
	}
	update_history_length(); /* updates g_custom_history_length */
}
