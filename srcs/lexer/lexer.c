/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_stdlib.h"
#include "libft.h"

static char	*trim_copy(const char *line);
size_t		count_qmark_extra(const char *in);
int			needs_home(const char *in, const char *home);

/* forward decl from expand_basic.c */
char		*expand_basic(const char *in);

/* helpers to recognize chain operators */
static int	is_op_at(const char *s, size_t i)
{
	return ((s[i] == '&' && s[i + 1] == '&')
		|| (s[i] == '|' && s[i + 1] == '|'));
}

static size_t	count_tokens_with_ops(const char *s)
{
	size_t	i = 0, n = 0;

	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break;
		if (is_op_at(s, i))
		{
			n++;
			i += 2;
			continue;
		}
		n++;
		while (s[i] && s[i] != ' ' && !is_op_at(s, i))
			i++;
	}
	return (n);
}

static char	*dup_token(const char *s, size_t start, size_t end)
{
	if (end <= start)
		return (ft_strdup(""));
	return (ft_substr(s, (unsigned int)start, end - start));
}

static void	free_vec(char **v)
{
	size_t i = 0;
	if (!v) return;
	while (v[i])
	{
		free(v[i]);
		i++;
	}
	free(v);
}

char	**lex_line(const char *line)
{
	char	*trimmed;
	char	**tokens;
	size_t	tcount, i = 0, k = 0;

	trimmed = trim_copy(line);
	if (!trimmed || !*trimmed)
		return (free(trimmed), NULL);
	tcount = count_tokens_with_ops(trimmed);
	tokens = (char **)ft_calloc(tcount + 1, sizeof(char *));
	if (!tokens)
		return (free(trimmed), NULL);
	while (trimmed[i])
	{
		while (trimmed[i] == ' ')
			i++;
		if (!trimmed[i])
			break;
		if (is_op_at(trimmed, i))
		{
			tokens[k++] = ft_substr(trimmed, (unsigned int)i, 2);
			i += 2;
		}
		else
		{
			size_t start = i;
			while (trimmed[i] && trimmed[i] != ' ' && !is_op_at(trimmed, i))
				i++;
			tokens[k] = dup_token(trimmed, start, i);
			if (!tokens[k])
				return (free(trimmed), free_vec(tokens), NULL);
			/* apply basic expansion (~ and $?) per token */
			{
				char *expanded = expand_basic(tokens[k]);
				if (expanded)
				{
					free(tokens[k]);
					tokens[k] = expanded;
				}
			}
			k++;
		}
	}
	tokens[k] = NULL;
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
