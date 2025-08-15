/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:27:56 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:52:41 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scanf.h"
#include <ctype.h>

int parse_string(t_scanf_data *data, va_list args)
{
    char    *dest;
    int     c;
    int     len;

    len = 0;
    dest = va_arg(args, char*);
    skip_whitespace(data);
    while ((c = get_char(data)) != EOF && !isspace((unsigned char)c))
        dest[len++] = (char)c;
    if (c != EOF)
        unget_char(data, c);
    dest[len] = '\0';
    return (len > 0) ? 1 : 0;
}
