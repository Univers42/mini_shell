/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:01 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 21:09:46 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	bin_echo(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("echo builtin called\n");
	return (0);
}
