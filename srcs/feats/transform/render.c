/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:41:23 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 14:13:59 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

/* Provide globals used by the renderer and prompt logic. */
t_render_mode g_render_mode = RENDER_FANCY;
int           g_last_status = 0;