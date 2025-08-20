/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:22:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:02:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * `api_load` -> `load_from_fd` -> `handle_loaded_line` -> `dll_push_tail_line`
 * those commands together load all commands line bby line into nodes of the 
 * history list.
 */
/* Helpers for load */

static void	handle_loaded_line(char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (line[0] != '\0')
	{
		dll_push_tail_line(line);
		add_history(line);
	}
}

static void	load_from_fd(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		handle_loaded_line(line);
		free(line);
		line = get_next_line(fd);
	}
}

void	api_load(void)
{
	t_history_state	*st;
	int				fd;

	st = access_hist_state();
	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;
	fd = open(st->histfile, O_RDONLY);
	if (fd < 0)
		return ;
	load_from_fd(fd);
	close(fd);
	update_history_length();
}
