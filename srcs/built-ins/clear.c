/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:57:14 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 22:13:18 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <unistd.h>

// Clear screen using ANSI escape sequences
// \033[2J clears entire screen
// \033[H moves cursor to top-left (1,1)
// \033[3J clears scrollback buffer (on supported terminals)
int bin_clear(char **args, int flags, t_env *env)
{
    (void)args;
    (void)flags;
    (void)env;
    const char *seq = "\033[2J\033[H\033[3J";
    ssize_t wr;

    wr = write(STDOUT_FILENO, seq, sizeof("\033[2J\033[H\033[3J") - 1);
    (void)wr;
    return (0);
}
