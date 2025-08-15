/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:27:11 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:52:40 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scanf.h"

int parse_char(t_scanf_data *data, va_list args)
{
    int c = get_char(data);
    char *dest = va_arg(args, char*);
    if (c == EOF)
        return 0;
    *dest = (char)c;
    return 1;
}
