/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:51:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 14:43:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * @brief in order to correctly hanndle history file path that use
 * shortcuts like `~`, it expands paths such as  `~/myhist` to the full
 * absolute path `/home/user/mmyhist`, ensuring the history file
 * is saved annd loaded fromm the intended location regardless of the user's 
 * ennvironment
 * 
 */
const char	*expand_hist_path(const char *name, char *out, size_t outsz)
{
	const char	*home;
	size_t		len;

	if (!name || !out || outsz == 0)
		return (NULL);
	home = getenv("HOME");
	if (name[0] == '~' && home && *home)
	{
		if (name[1] == '/' || name[1] == '\0')
		{
			if (ft_snprintf(out, outsz, "%s/%s", home, name + 1) <= 0)
				return (NULL);
			return (out);
		}
	}
	len = ft_strclen(name, '\0');
	if (len > outsz - 1)
		len = outsz - 1;
	ft_strlcpy(out, name, outsz);
	out[len] = '\0';
	return (out);
}
