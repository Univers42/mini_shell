/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:42:19 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/21 03:07:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_err	parse_tokens(char **toks, t_cmdline *out);

// Force-link reference to ensure the TU rebuilds with updated builtins
void	force_link_history_ref(void)
{
	(void)&bin_history;
}

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
