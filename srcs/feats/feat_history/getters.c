/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 22:14:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 22:14:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/* --- api_init, unchanged --- */

int	api_init(const t_history_opts *opts, char **envp)
{
	t_history_state		*st;
	const char			*env_hist;
	const char			*env_size;
	long				v;

	st = S();
	(void)envp;
	if (!st)
		return (1);
	st->persist = true;
	st->histsize = DEFAULT_HISTSIZE;
	st->histfile[0] = '\0';
	env_hist = getenv("MS_HISTORY");
	env_size = getenv("MS_HISTSIZE");
	if (opts)
	{
		st->persist = opts->persist;
		if (opts->histsize != 0)
			st->histsize = opts->histsize;
		if (opts->histfile && *opts->histfile)
			expand_hist_path(opts->histfile, st->histfile, sizeof(st->histfile));
	}
	if (env_hist && *env_hist)
		expand_hist_path(env_hist, st->histfile, sizeof(st->histfile));
	if (st->histfile[0] == '\0')
		expand_hist_path("~/" DEFAULT_HISTFILE, st->histfile, sizeof(st->histfile));
	if (env_size && *env_size)
	{
		v = ft_atol(env_size);
		if (v > 0 && v <= INT_MAX)
			st->histsize = (int)v;
	}
	if (st->histsize > 0)
		custom_stifle_history(st->histsize);
	else
		custom_unstifle_history();
	st->initialized = true;
	return (0);
}

/* --- api_load, modified to use open/read/close --- */

void	api_load(void)
{
	t_history_state	*st;
	int				fd;
	char			*line;

	st = S();
	if (!st || !st->initialized)
		return;
	if (!st->persist || st->histfile[0] == '\0')
		return;

	fd = open(st->histfile, O_RDONLY);
	if (fd < 0)
		return;

	line = get_next_line(fd);
	while (line)
	{
		size_t len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		/* mirror into our DLL and readline (allowed) */
		if (line[0] != '\0')
		{
			dll_push_tail_line(line);
			add_history(line);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	update_history_length();
}

/* --- api_save, modified to use open/write/close --- */

void	api_save(void)
{
	t_history_state	*st;
	int				fd;
	t_dll_node		*node;

	st = S();
	if (!st || !st->initialized)
		return;
	if (!st->persist || st->histfile[0] == '\0')
		return;

	fd = open(st->histfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return;

	node = st->list ? st->list->head : NULL;
	while (node)
	{
		const char *s = (const char *)node->data;
		if (s && *s)
		{
			size_t left = ft_strlen(s);
			const char *p = s;
			ssize_t wr;
			while (left > 0)
			{
				wr = write(fd, p, left);
				if (wr <= 0)
					break;
				p += (size_t)wr;
				left -= (size_t)wr;
			}
			(void)write(fd, "\n", 1);
		}
		node = node->next;
	}
	close(fd);
}
