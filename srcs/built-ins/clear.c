/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:57:14 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/20 22:09:55 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <unistd.h>

// Clear screen using ANSI escape sequences
// \033[2J clears entire screen
// \033[H moves cursor to top-left (1,1)
// \033[3J clears scrollback buffer (on supported terminals)
int	bin_clear(char **args, int flags, t_env *env)
{
	const char	*seq;
	ssize_t		wr;

	(void)args;
	(void)flags;
	(void)env;
	seq = "\033[2J\033[H\033[3J";
	wr = write(STDOUT_FILENO, seq, sizeof("\033[2J\033[H\033[3J") - 1);
	(void)wr;
	return (0);
}
