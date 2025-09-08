/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:04:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/21 03:09:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"

static int	parse_dash_flags(const char *tok, int valid, int *flags)
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

static int	parse_bare_as_flags(const char *tok, int valid, int *flags)
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

static int	find_builtin_idx(const char *cmd)
{
	int			i;
	t_builtins	*bins;

	if (!cmd || !*cmd)
		return (BIN_NOT_FOUND);
	bins = access_builtins();
	i = 0;
	while (bins[i].name)
	{
		if (ft_strcmp(cmd, bins[i].name) == 0)
			return (i);
		i++;
	}
	return (BIN_NOT_FOUND);
}

static t_parse_err	scan_tokens(char **toks, t_cmdline *out, int valid)
{
	int				i;
	t_parse_state	st;

	st = ST_CMD;
	i = 1;
	while (st != ST_DONE && st != ST_ERROR && i < out->argc)
	{
		if (toks[i][0] == '-' && toks[i][1] != '\0')
		{
			if (!parse_dash_flags(toks[i], valid, &out->flags))
				st = ST_ERROR;
			else
				st = ST_FLAGS;
		}
		else if (!parse_bare_as_flags(toks[i], valid, &out->flags))
			st = ST_ARGS;
		i++;
	}
	if (st == ST_ERROR)
		return (PARSE_INVALID_FLAG);
	return (PARSE_OK);
}

t_parse_err	ms_parse_tokens(char **toks, t_cmdline *out)
{
	int	valid;

	out->argc = count_tokens(toks);
	if (out->argc <= 0)
		return (PARSE_EMPTY);
	out->bin_idx = find_builtin_idx(toks[0]);
	if (out->bin_idx == BIN_NOT_FOUND)
		return (PARSE_NOT_BUILTIN);
	out->flags = 0;
	valid = access_builtins()[out->bin_idx].valid_flags;
	return (scan_tokens(toks, out, valid));
}
