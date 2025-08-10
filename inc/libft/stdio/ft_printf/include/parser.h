/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:46:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/05 00:46:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdarg.h>
# include "writer.h"

typedef struct s_token_meta
{
	int		flags;
	int		width;
	int		precision;
	char	specifier;
}	t_token_meta;

typedef struct s_parser
{
	size_t			index;
	const char		*format;
	va_list			*ap;
	t_writer		*writer;
	t_token_meta	token_meta;
}	t_parser;

int	parser_parse_and_write(
		t_parser *parser,
		const char *format_str,
		va_list *params,
		t_writer *buf_out
		);

#endif