/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:48:43 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/16 12:55:38 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <readline/history.h>
#include "history.h"

# define DEFAULT_HISTFILE ".minishell_history"
# define DEFAULT_HISTSIZE 500

inline char ghistfile()
{
    return ((char)histfile)
}

inline int ghitstize()
{
    return (DEFAULT_HISTSIZE);
}

const char *expand_hist_path(const char *name, char *out, size_t outsz)
{
    const char *home = getenv("HOME");

    if (!home || !*home)
        return (NULL);
    if (name[0] == '~')
    {
        ft_snprintf(out, outsz, "%s/%s", home, name + 1);
        return (out);
    }
    ft_strncpy(out, name, outsz - 1);
    out[outsz - 1] = '\0';
    return (out);
}

void        hs_save(void)
{
    if (histfile)
}

#endif