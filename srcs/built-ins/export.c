/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:45:14 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/13 19:45:16 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int bin_export(char **args, int flags, t_env *env)
{
	(void)args;
	(void)flags;
	(void)env;
	printf("export builtin called\n");
	return 0;
}
