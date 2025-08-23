/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:40:38 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/23 20:41:33 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

static int cmp_env(const void *a, const void *b) {
    const char *sa = *(const char * const *)a;
    const char *sb = *(const char * const *)b;

    const char *ea = ft_strchr(sa, '=');
    if (ea == NULL)
        ea = sa + ft_strlen(sa);
    const char *eb = ft_strchr(sb, '=');
    if (eb == NULL)
        eb = sb + strlen(sb);

    size_t la = (size_t)(ea - sa);
    size_t lb = (size_t)(eb - sb);

    size_t minlen;
    if (la < lb)
        minlen = la;
    else
        minlen = lb;

    int cmp = strncmp(sa, sb, minlen);
    if (cmp != 0)
        return cmp;

    if (la < lb)
        return -1;
    else if (la > lb)
        return 1;
    else
        return 0;
}
