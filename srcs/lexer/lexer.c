/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:28 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 12:54:24 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_stdlib.h"
#include "libft.h"

static char	*trim_copy(const char *line);

char	**lex_line(const char *line)
{
	char	*trimmed;
	char	**tokens;

	trimmed = trim_copy(line);
	if (!trimmed || !*trimmed)
		return (free(trimmed), NULL);
	tokens = ft_split(trimmed, ' ');
	free(trimmed);
	return (tokens);
}

static char *trim_copy(const char *line)
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
