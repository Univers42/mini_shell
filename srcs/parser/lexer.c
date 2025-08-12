/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:28 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/12 14:36:14 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_stdlib.h"

static char *ms_trim_copy(const char *line)
{
	t_addr	ptr;
	char	*copy;
	size_t	len;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	copy = (char *)malloc(len + 1);
	if (!copy)
		return (NULL);
	ft_strlcpy(copy, line, len + 1);
	ptr = (t_addr)copy;
	ft_super_trim(&ptr, TRIM_SPACES);
	return ((char *)ptr);
}

char	**ms_lex_line(const char *line)
{
	char	*trimmed;
	char	**tokens;

	trimmed = ms_trim_copy(line);
	if (!trimmed || !*trimmed)
		return (free(trimmed), NULL);
	tokens = ft_split(trimmed, ' ');
	free(trimmed);
	return (tokens);
}

int	ms_count_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

void	ms_free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}
