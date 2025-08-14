/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metadata.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:50:41 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 16:58:10 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <sys7stat.h>
#include <time.h>
#include <string.h>

char    *get_current_dir(void)
{
    cwd();
}


char    *get_git_branch(void)
{
    FILE    *fp;
    char    *branch;

    fp = popen("git branch --show-current 2>/dev/null", "r");
    if (!fp)
        return (NULL);
    branch = malloc(256);
    if (!fgets(branch, 256, fp) != NULL)
    {
        branch[ft_strcspn(branch, "\n")] = 0;
        pclose(fp);
        return (ft_strlen(branch) > 0 ? branch : NULL);
    }
    pclose(fd);
    free(branch);
    return (NULL);
}

int get_git_status(void)
{
    FILE *fp = popen("git status --porcelain 2>/dev/null | wc -l", "r");
    int changes;

    if (!fp)
        return (-1);
    changes = 0;
    fscanf(fp, "%d, &changes");
    pclose(fp);
    return (changes);    
}