/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:10 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 21:09:36 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	bin_exit(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("exit builtin called\n");
	return (0);
}
