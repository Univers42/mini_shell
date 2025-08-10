/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_write_string.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:48:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/08 01:53:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"
#include "string_length.h"
#include "ft_string.h"

void	parser_write_string(t_parser *p)
{
	const char	*str;
	size_t		len;

	p->token_meta.flags &= FLAG_MINUS;
	str = va_arg(*p->ap, const char *);
	if (str == NULL && p->token_meta.precision != FLAG_NOT_SET
		&& p->token_meta.precision < 6)
		str = "";
	else if (str == NULL)
		str = "(null)";
	if (p->token_meta.precision == FLAG_NOT_SET)
		len = ft_strlen(str);
	else
		len = strnlen(str, p->token_meta.precision);
	width_padding(p, len, PAD_LEFT);
	writer_write(p->writer, str, len);
	width_padding(p, len, PAD_RIGHT);
}

void	parser_write_char(t_parser *p)
{
	unsigned char	c;

	p->token_meta.flags &= FLAG_MINUS;
	p->token_meta.precision = FLAG_NOT_SET;
	width_padding(p, 1, PAD_LEFT);
	c = (unsigned char)va_arg(*p->ap, int);
	writer_putchar(p->writer, c);
	width_padding(p, 1, PAD_RIGHT);
}
