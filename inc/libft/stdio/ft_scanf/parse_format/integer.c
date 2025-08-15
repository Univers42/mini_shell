/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:30:27 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:52:41 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scanf.h"
#include <ctype.h>

int parse_integer(t_scanf_data *data, va_list args)
{
    int c;
    int sign = 1;
    long num = 0;
    int digits = 0;
    int base = 10;
    int *dest = va_arg(args, int*);

    skip_whitespace(data);
    c = get_char(data);
    if (c == '+' || c == '-')
    {
        if (c == '-') sign = -1;
        c = get_char(data);
    }
    if (c == '0')
    {
        int n = get_char(data);
        if (n == 'x' || n == 'X')
        {
            base = 16;
            c = get_char(data);
        }
        else
        {
            base = 8;
            if (n != EOF)
                unget_char(data, n);
            c = get_char(data);
        }
    }
    /* parse according to base */
    if (base == 10)
    {
        while (c != EOF && isdigit((unsigned char)c))
        {
            num = num * 10 + (c - '0');
            digits++;
            c = get_char(data);
        }
    }
    else if (base == 8)
    {
        while (c != EOF && (c >= '0' && c <= '7'))
        {
            num = num * 8 + (c - '0');
            digits++;
            c = get_char(data);
        }
    }
    else /* base 16 */
    {
        while (c != EOF)
        {
            int v = hexval(c);
            if (v < 0) break;
            num = num * 16 + v;
            digits++;
            c = get_char(data);
        }
    }
    if (c != EOF)
        unget_char(data, c);
    if (digits == 0)
        return 0;
    *dest = (int)(num * sign);
    return 1;
}