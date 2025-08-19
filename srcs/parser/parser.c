/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 12:46:56 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Force-link reference to ensure the TU rebuilds with updated builtins
static void	ms_force_link_history_ref(void)
{
	(void)&bin_history;
}

int	find_builtin_idx(const char *cmd)
{
	int			i;
	t_builtins	*bins;

	ms_force_link_history_ref();
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

static int	parse_dash_flags(const char *tok, int valid, int *flags)
{
	int	j;
	int	bit;

	j = 1;
	while (tok[j])
	{
		bit = ms_flag_bit(tok[j]);
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
		bit = ms_flag_bit(tok[j]);
		if (!bit || !(valid & bit))
			return (0);
		acc |= bit;
		j++;
	}
	*flags |= acc;
	return (1);
}

static t_parse_err	parse_tokens(char **toks, t_cmdline *out)
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

t_parse_err	ms_parse_line(const char *line, t_cmdline *out)
{
	char	**tokens;
	t_parse_err err;

	if (!out)
		return (PARSE_EMPTY);
	ft_bzero(out, sizeof(*out));
	tokens = lex_line(line);
	if (!tokens)
		return (PARSE_EMPTY);
	out->argv = tokens;
	err = parse_tokens(tokens, out);
	out->err = err;
	return (err);
}

void	ms_cmdline_free(t_cmdline *cmd)
{
	if (!cmd)
		return ;
	free_tokens(cmd->argv);
	ft_bzero(cmd, sizeof(*cmd));
}
