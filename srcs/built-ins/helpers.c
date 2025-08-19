/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:56:32 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 13:03:17 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

int is_flag_set(int flags, int bit)
{
    return ((flags & bit) != 0);
}

int	flag_bit(char ch)
{
	int	bit;

	bit = flag_from_char(ch);
	if (bit)
		return (bit);
	if (ch >= 'A' && ch <= 'Z')
		return (flag_from_char((char)(ch - 'A' + 'a')));
	return (0);
}
