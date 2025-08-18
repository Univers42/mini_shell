/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:55:11 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/18 21:28:39 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

/**
[User types]   echo -e "\tHello world!"
      ↓
[Lexer]        Tokenizes: ["echo", "-e", "\tHello world!"]
               → Keeps spaces in quoted strings
               → Removes the quotes
      ↓
[Parser]       Expands variables, globs, tilde
      ↓
[Executor]     Calls builtin_echo(argv = ["echo", "-e", "\tHello world!"])
      ↓
[echo]         Sees argv[2] = "\tHello world!"
               If -e → parse backslashes → "\t" → '\t' char
               Prints with tabs/newlines accordingly
*/

int bin_echo(char **args, int flags, t_env *env)
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

void	echo_output(char **processed, int flags)
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

void	apply_handlers(char **args, char **processed, int flags)
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
