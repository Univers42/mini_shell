/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:42:19 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 15:45:21 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_err	parse_line(const char *line, t_cmdline *out)
{
	char		**tokens;
	t_parse_err	err;

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

void	cmdline_free(t_cmdline *cmd)
{
	if (!cmd)
		return ;
	free_tokens(cmd->argv);
	ft_bzero(cmd, sizeof(*cmd));
}

int	find_builtin_idx(const char *cmd)
{
	int			i;
	t_builtins	*bins;

	force_link_history_ref();
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
