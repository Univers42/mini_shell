/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:30 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 16:45:06 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

static char	*build_tilde_path(const char *cwd, const char *home)
{
	size_t	home_len;
	size_t	rel_len;
	char	*out;

	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) != 0)
		return (NULL);
	if (cwd[home_len] != '/' && cwd[home_len] != '\0')
		return (NULL);
	rel_len = ft_strlen(cwd + home_len);
	out = (char *)malloc(rel_len + 2);
	if (!out)
		return (NULL);
	if (!ft_snprintf(out, rel_len + 2, "~%s", cwd + home_len))
		ft_putendl_fd("buffer of snprintf empty", 2);
	return (out);
}

char	*get_cwd_pretty(void)
{
	char	*cwd;
	char	*home;
	char	*pretty;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	home = getenv("HOME");
	if (home)
	{
		pretty = build_tilde_path(cwd, home);
		if (pretty)
			return (free(cwd), pretty);
	}
	return (cwd);
}

char *get_cwd(void)
{
	char 	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	return (cwd);
}

int	bin_pwd(char **args, int flags, t_env *env)
{
	char	*cwd;

	(void)args;
	(void)flags;
	(void)env;
	cwd = get_cwd_pretty();
	if (!cwd)
		return (1);
	return (ft_putendl_fd(cwd, STDOUT_FILENO), free(cwd), 0);
}
