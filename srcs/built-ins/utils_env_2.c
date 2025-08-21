/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielm3 <danielm3@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:15:06 by danielm3          #+#    #+#             */
/*   Updated: 2025/08/21 00:18:06 by danielm3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

/*
** is_valid_identifier
** ------------------
** Check if a string is a valid shell variable identifier.
** Must start with letter/underscore, followed by letters/digits/underscores.
** Used by unset/export to validate variable names.
** Returns 1 if valid, 0 otherwise.
*/
int	is_valid_identifier(const char *s)
{
	size_t	i;

	if (!s || !s[0] || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
