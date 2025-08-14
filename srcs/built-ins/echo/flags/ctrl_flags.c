/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:58:53 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 14:59:23 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

/* Table: lower priority value runs first */
const t_echo_rule	*echo_rules(void)
{
	static const t_echo_rule rules[] = {
		{ FLAG_E, 10, handle_escapes },
		{ FLAG_U, 20, handle_uppercase },
		{ 0, 0,  NULL }
	};
	return (rules);
}