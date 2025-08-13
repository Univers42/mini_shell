/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 01:16:28 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 01:18:53 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

static size_t	str_len(const char *s)
{
	size_t i = 0;
	if (!s) return 0;
	while (s[i]) i++;
	return i;
}

static char	*dup_cstr(const char *s)
{
	size_t	len;
	char	*out;
	size_t	i;

	len = str_len(s);
	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (i < len)
	{
		out[i] = s[i];
		i++;
	}
	out[len] = '\0';
	return (out);
}

static int	is_flag_set(int flags, int bit)
{
	return ((flags & bit) != 0);
}

static int	ms_flag_bit(char ch)
{
	int	bit;

	bit = flag_from_char(ch);
	if (bit)
		return (bit);
	if (ch >= 'A' && ch <= 'Z')
		return (flag_from_char((char)(ch - 'A' + 'a')));
	return (0);
}

/* Compute index of first non-flag argument (skip leading -[neEuU] groups) */
static size_t	echo_args_start(char **args, int valid_flags)
{
	size_t	i;
	size_t	j;
	int		bit;

	i = 1; /* skip command name */
	while (args && args[i] && args[i][0] == '-' && args[i][1] != '\0')
	{
		j = 1;
		while (args[i][j])
		{
			bit = ms_flag_bit(args[i][j]);
			if (!bit || !(valid_flags & bit))
				return (i); /* not a flag token: stop skipping */
			j++;
		}
		i++; /* full flag token consumed */
	}
	return (i);
}

/* Transform C-like escapes in processed_args[i], in place via replace buffer */
static void	handle_escapes(char **args, char **processed_args)
{
	size_t	i;

	(void)args;
	i = 1;
	while (processed_args && processed_args[i])
	{
		const char	*src = processed_args[i];
		size_t		j = 0;
		size_t		out_len = 0;
		char		*out;

		/* worst-case size == len(src) */
		out = (char *)malloc(str_len(src) + 1);
		if (!out)
			return;
		while (src[j])
		{
			if (src[j] == '\\' && src[j + 1])
			{
				j++;
				if (src[j] == 'n') out[out_len++] = '\n';
				else if (src[j] == 't') out[out_len++] = '\t';
				else if (src[j] == 'r') out[out_len++] = '\r';
				else if (src[j] == 'v') out[out_len++] = '\v';
				else if (src[j] == 'f') out[out_len++] = '\f';
				else if (src[j] == 'a') out[out_len++] = '\a';
				else if (src[j] == 'b') out[out_len++] = '\b';
				else out[out_len++] = src[j];
				j++;
				continue;
			}
			out[out_len++] = src[j++];
		}
		out[out_len] = '\0';
		free(processed_args[i]);
		processed_args[i] = out;
		i++;
	}
}

/* Uppercase all processed_args characters */
static void	handle_uppercase(char **args, char **processed_args)
{
	size_t	i;
	size_t	j;

	(void)args;
	i = 1;
	while (processed_args && processed_args[i])
	{
		j = 0;
		while (processed_args[i][j])
		{
			char c = processed_args[i][j];
			if (c >= 'a' && c <= 'z')
				processed_args[i][j] = (char)(c - ('a' - 'A'));
			j++;
		}
		i++;
	}
}

/* Table: lower priority value runs first */
static const t_echo_rule	*echo_rules(void)
{
	static const t_echo_rule rules[] = {
		/* Escapes should run before uppercase for predictable results */
		{ FLAG_E,          10, handle_escapes },
		/* FLAG_E_DISABLE handled by driver (skip escapes) */
		{ FLAG_U,          20, handle_uppercase },
		{ 0,               0,  NULL }
	};
	return (rules);
}

/* duplicate argv but exclude leading flag tokens; keep index 0 as command */
static char	**dup_args_vector_from(char **args, size_t start)
{
	size_t	argc;
	size_t	outc;
	size_t	i;
	size_t	o;
	char	**out;

	argc = 0;
	while (args && args[argc])
		argc++;
	/* out: argv[0] + all args from start..end */
	outc = 1 + (start < argc ? (argc - start) : 0);
	out = (char **)malloc(sizeof(char *) * (outc + 1));
	if (!out)
		return (NULL);
	out[0] = dup_cstr(args[0]);
	if (!out[0])
		return (free(out), NULL);
	o = 1;
	i = start;
	while (i < argc)
	{
		out[o] = dup_cstr(args[i]);
		if (!out[o])
		{
			while (o > 0)
				free(out[--o]);
			free(out);
			return (NULL);
		}
		o++;
		i++;
	}
	out[outc] = NULL;
	return (out);
}

static void	free_args_vector(char **v)
{
	size_t i = 0;
	if (!v) return;
	while (v[i])
		free(v[i++]);
	free(v);
}

static void	apply_handlers(char **args, char **processed, int flags)
{
	const t_echo_rule	*rules;
	size_t				i;

	rules = echo_rules();
	i = 0;
	while (rules[i].handler)
	{
		if (rules[i].flag == FLAG_E)
		{
			if (is_flag_set(flags, FLAG_E) && !is_flag_set(flags, FLAG_E_DISABLE))
				rules[i].handler(args, processed);
		}
		else if (is_flag_set(flags, rules[i].flag))
			rules[i].handler(args, processed);
		i++;
	}
}

static void	echo_output(char **processed, int flags)
{
	size_t	i;

	i = 1;
	while (processed && processed[i])
	{
		ft_printf("%s", processed[i]);
		if (processed[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!is_flag_set(flags, FLAG_N))
		ft_printf("\n");
}

int	bin_echo(char **args, int flags, t_env *env)
{
	char	**processed;
	size_t	start;
	int		valid;

	(void)env;
	if (!args || !args[0])
		return (0);
	valid = FLAG_N | FLAG_E | FLAG_E_DISABLE | FLAG_U;
	start = echo_args_start(args, valid);
	processed = dup_args_vector_from(args, start);
	if (!processed)
		return (1);
	apply_handlers(args, processed, flags);
	echo_output(processed, flags);
	free_args_vector(processed);
	return (0);
}