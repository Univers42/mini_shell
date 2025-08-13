/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:37 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 19:45:38 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int bin_unset(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("unset builtin called\n");
	return 0;
}
