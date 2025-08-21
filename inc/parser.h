/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/* forward to avoid circular deps */
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

/* public API */
t_parse_err	ms_parse_line(const char *line, t_cmdline *out);
void		ms_cmdline_free(t_cmdline *cmd);

/* tokens -> cmdline parse (implemented in parse_core.c) */
t_parse_err	ms_parse_tokens(char **toks, t_cmdline *out);

#endif