/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:52:28 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 15:41:24 by syzygy           ###   ########.fr       */
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

/**
 * @brief The crash comes from freeing a pointer 
 * that no longer points to the start of the allocated buffer
 * why ? trim_copy allocates a buffer,
 * then ft_super_trim moves teh pointer into the middle of that buffer
 * lex_line later frees the returned pointer,
 * which is invalid when it's not the original base
 * in trim_copy, duplicate the trimmed view into a fresh string,
 * free the original base, and return
 * the duplicate. Now lex_line can safely free it 
 Duplicate the trimmed view, then free the original base pointer
 make trim copy return a fresh duplicate of the trimmed substring
 and freee the original base buffer 
 */
static char	*trim_copy(const char *line)
{
	t_addr	ptr;
	char	*copy;
	char	*out;
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
	out = ft_strdup((char *)ptr);
	free(copy);
	return (out);
}
