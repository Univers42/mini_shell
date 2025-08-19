/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:29:26 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 14:12:51 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

//to avoid circular dependencies
typedef struct s_cmdline	t_cmdline;

/* Parsing state machine */
typedef enum e_parse_state
{
	ST_START = 0,
	ST_CMD,
	ST_FLAGS,
	ST_ARGS,
	ST_DONE,
	ST_ERROR
}	t_parse_state;

/* Parsing error codes */
typedef enum e_parse_err
{
	PARSE_OK = 0,
	PARSE_EMPTY,
	PARSE_NOT_BUILTIN,
	PARSE_INVALID_FLAG
}	t_parse_err;

t_parse_err	parse_line(const char *line, t_cmdline *out);
void		cmdline_free(t_cmdline *cmd);

#endif