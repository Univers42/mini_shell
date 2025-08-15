/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:29:34 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:54:26 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scanf.h"
#include <stddef.h>

/* keep your jump table design: table indexed by char value */
t_scanning *get_parser_method(void)
{
    static t_scanning table[256] = { NULL };

    /* initialize once */
    if (table['c'] == NULL && table['s'] == NULL && table['d'] == NULL && table['i'] == NULL)
    {
        table['c'] = parse_char;
        table['s'] = parse_string;
        table['d'] = parse_decimal;
        table['i'] = parse_integer;
    }
    return table;
}
