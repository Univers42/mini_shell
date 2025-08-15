/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decimal.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:28:46 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:52:40 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scanf.h"
#include <ctype.h>

int parse_decimal(t_scanf_data *data, va_list args)
{
    int c;
    int sign = 1;
    long num = 0;
    int digits = 0;
    int *dest = va_arg(args, int*);

    skip_whitespace(data);
    c = get_char(data);
    if (c == '+' || c == '-')
    {
        if (c == '-') sign = -1;
        c = get_char(data);
    }
    while (c != EOF && isdigit((unsigned char)c))
    {
        num = num * 10 + (c - '0');
        digits++;
        c = get_char(data);
    }
    if (c != EOF)
        unget_char(data, c);
    if (digits == 0)
        return 0;
    *dest = (int)(num * sign);
    return 1;
}