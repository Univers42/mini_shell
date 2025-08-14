/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:30 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 16:16:41 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "builtins.h" // if you have ft_strlen etc.

char *get_cwd(void)
{
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
        return strdup("~"); // fallback if getcwd fails

    char *home = getenv("HOME");
    if (home && strncmp(cwd, home, strlen(home)) == 0)
    {
        // Allocate space for "~" + rest of path + null terminator
        size_t relative_len = strlen(cwd) - strlen(home);
        char *relative = malloc(relative_len + 2); // "~" + rest + '\0'
        if (!relative)
        {
            free(cwd);
            return NULL;
        }
        // Build string: "~" + (cwd after home)
        sprintf(relative, "~%s", cwd + strlen(home));
        free(cwd);
        return relative;
    }
    return cwd; // just return full path if not in HOME
}


int bin_pwd(char **args, int flags, t_env *env)
{
    char *cwd;

    (void)args;
    (void)flags;
    (void)env;

    cwd = get_cwd(); // use the new helper here
    if (!cwd)
        return 1;

    ft_putendl_fd(cwd, 1); // print it with newline
    free(cwd);
    return 0;
}
