/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:44:51 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/14 15:48:12 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>

// Military theme colors
#define MILITARY_GREEN "\033[38;5;22m"
#define MILITARY_AMBER "\033[38;5;214m"
#define MILITARY_RED "\033[38;5;196m"
#define MATRIX_GREEN "\033[38;5;46m"

typedef enum e_render_mode
{
    RENDER_SIMPLE;
    RENDER_FANCY;
    RENDER_MILITARY;
    RENDER_MATRIX;
    RENDER_HACKER;
}t_render_mode;

typedef struct s_congig_render
{
    t_render_mode   mode;
    int             show_git;
    int             show_time;
    int             show_user;
    int             animated;
}                   t_config_render;