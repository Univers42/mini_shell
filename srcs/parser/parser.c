/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/21 03:08:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_builtin_idx(const char *cmd);

int	parse_dash_flags(const char *tok, int valid, int *flags)
{
	int	j;
	int	bit;

	j = 1;
	while (tok[j])
	{
		bit = flag_bit(tok[j]);
		if (!bit || !(valid & bit))
			return (0);
		*flags |= bit;
		j++;
	}
	return (1);
}

int	parse_bare_as_flags(const char *tok, int valid, int *flags)
{
	int	j;
	int	bit;
	int	acc;

	acc = 0;
	j = 0;
	while (tok[j])
	{
		bit = flag_bit(tok[j]);
		if (!bit || !(valid & bit))
			return (0);
		acc |= bit;
		j++;
	}
	*flags |= acc;
	return (1);
}

/**
 * tok is non-NULL and non-empty.
 * bare token: if it parses as flags, keep flags state; otherwise this is an arg.
 */
t_parse_state	handle_token(const char *tok, int valid, int *flags)
{
	if (tok[0] == '-' && tok[1] != '\0')
	{
		if (!parse_dash_flags(tok, valid, flags))
			return (ST_ERROR);
		return (ST_FLAGS);
	}
	if (!parse_bare_as_flags(tok, valid, flags))
		return (ST_ARGS);
	return (ST_FLAGS);
}

t_parse_err	parse_tokens(char **toks, t_cmdline *out)
{
	int				i;
	int				valid;
	t_parse_state	st;

	out->argc = count_tokens(toks);
	if (out->argc <= 0)
		return (PARSE_EMPTY);
	out->bin_idx = find_builtin_idx(toks[0]);
	if (out->bin_idx == BIN_NOT_FOUND)
		return (PARSE_NOT_BUILTIN);
	out->flags = 0;
	st = ST_CMD;
	valid = access_builtins()[out->bin_idx].valid_flags;
	i = 1;
	while (i < out->argc && st != ST_ERROR)
	{
		st = handle_token(toks[i], valid, &out->flags);
		if (st == ST_ARGS)
			break ;
		i++;
	}
	if (st == ST_ERROR)
		return (PARSE_INVALID_FLAG);
	return (PARSE_OK);
}
