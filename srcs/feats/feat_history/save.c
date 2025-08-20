/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:21:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 21:34:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * low-level helper that is used to write the entire contents of a buffer to
 * a fd.. handling partial writes.
 * in our implementation it is used by `save_node_line`and utlimately by
 * `api_save` to persist history liines to the history file
 */
void	write_all(int fd, const char *buf, size_t len)
{
	ssize_t		wr;
	const char	*p;
	size_t		left;

	p = buf;
	left = len;
	while (left > 0)
	{
		wr = write(fd, p, left);
		if (wr <= 0)
			break ;
		p += (size_t)wr;
		left -= (size_t)wr;
	}
}

/**
 * @param fd HIST_FILE fd
 * @param s a the command used
 * @note
 * it save each command by writing its string to the history file
 * using the file descriptor, followed by a newline. This way each
 * command is stored on a separate line in the file.
 * 
 */
void	save_node_line(int fd, const char *s)
{
	if (!s || !*s)
		return ;
	write_all(fd, s, ft_strlen(s));
	(void)write(fd, "\n", 1);
}

void	save_list_to_fd(int fd, t_dll_node *node)
{
	while (node)
	{
		save_node_line(fd, (const char *)node->data);
		node = node->next;
	}
}

/**
 * Whenn we use our  minnishell program, every command entered by the user
 * is added to
 * the internal history list (as nodes)
 * throughout the session, this list accumulates all commands.
 * whenn the program is about to exit (api_shutdown) api_save is called
 * save_list_to_fd is called to write every command from teh list to 
 * the histor file, one line per commmand
*/
void	api_save(void)
{
	const t_history_state	*st = access_hist_state();
	int						fd;
	t_dll_node				*head;

	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;
	fd = open(st->histfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return ;
	if (st->list)
		head = st->list->head;
	else
		head = NULL;
	save_list_to_fd(fd, head);
	close(fd);
}
