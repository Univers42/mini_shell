/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:28:49 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:54:08 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scanf.h"
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int get_char(t_scanf_data *data)
{
    ssize_t n;
    if (data->buf_pos >= data->buf_len)
    {
        n = read(data->fd, data->buffer, BUFFER_SIZE);
        if (n <= 0)
            return (EOF);
        data->buf_len = (int)n;
        data->buf_pos = 0;
    }
    return (unsigned char)data->buffer[data->buf_pos++];
}

void unget_char(t_scanf_data *data, int c)
{
    (void)c;
    if (data->buf_pos > 0)
        data->buf_pos--;
}

void skip_whitespace(t_scanf_data *data)
{
    int c;
    while ((c = get_char(data)) != EOF && isspace((unsigned char)c))
        ;
    if (c != EOF)
        unget_char(data, c);
}


