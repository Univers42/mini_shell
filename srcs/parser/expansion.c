/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:44:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 12:05:42 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_last_status;

static void	replace_home(char **str)
{
	const char	*home = getenv("HOME");
	if (!home || !*home || !str || !*str)
		return ;
	
}

// Replace all occurences of $? with the last status
static void replace_last_status(char *in, t_minishell *ms)
{
	
	
	
	
	
	
	char	*pos;



	
	char	num[32];
	char	*new_str;
	int		n;
	
	if (!str || !*str)
		return ;
	pos = *str;
	while ((pos = ft_strstr(pos, "$?")) != NULL)
	{
		n = ft_snprintf(num, sizeof(num), "%d", g_last_status);
		if (n > 0)
		{
			new_str = (char *)malloc(strlen(*str) + n);
			if (!new_str)
				return ;
			ft_memcpy(new_str, *str, pos - *str);
			ft_strcpy(new_str + (pos - *str), num);
			ft_strcpy(new_str + (pos - *str) + n, pos + 2);
			free(*str);
			*str = new_str;
		}
	}
}

/* very small expansion: replace all "$?" with last status,
	and a leading "~" or "~/"
	with $HOME. Returns malloc'ed string (caller frees).
*/
char    *expand_basic(const char *in)
{
	const char		*home = getenv("HOME");
	const size_t	in_len = strlen(in);
	size_t			extra;
	size_t			i;

	extra = 0;
	i = 0;
	while (++i < in_len)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			extra += 16;
			++i;
		}
	}
	if (home && (in[0] == '~') && (in[1] == '\0' || in[1] == '/'))
		extra += ft_strlen(home) + 1; /* replace '~' */

	char *out = (char *)malloc(in_len + extra + 1);
	if (!out)
		return ft_strdup(in);

	size_t o = 0;
	i = 0;

	/* handle leading ~ */
	if (home && in[0] == '~' && (in[1] == '\0' || in[1] == '/'))
	{
		size_t hl = strlen(home);
		memcpy(out + o, home, hl);
		o += hl;
		i = 1; /* skip '~' */
	}
	for (; i < in_len; ++i)
	{
		if (in[i] == '$' && in[i + 1] == '?')
		{
			char num[32];
			int n = snprintf(num, sizeof(num), "%d", g_last_status);
			if (n > 0)
			{
				memcpy(out + o, num, (size_t)n);
				o += (size_t)n;
			}
			++i; /* skip '?' */
		}
		else
		{
			out[o++] = in[i];
		}
	}
	out[o] = '\0';
	return out;
}

// change signature to receive shell env (t_env*), not argc/argv/envp
void	print_parse_error(const char *cmd, t_parse_err err, t_env *env)
{
	if (err == PARSE_NOT_BUILTIN)
	{
		// Split the user input into tokens and run as external command
		char **toks = ms_lex_line(cmd);
		if (toks)
		{
			int targc = ms_count_tokens(toks);
			// extract real envp from t_env* (which is a char***)
			char ***penv = (char ***)env;
			char **envp = (penv && *penv) ? *penv : NULL;
			(void)exec_internal(targc, toks, envp);
			ms_free_tokens(toks);
		}
		return;
	}
	else if (err == PARSE_INVALID_FLAG)
		fprintf(stderr, "Unknown command: %s\n", cmd);
}