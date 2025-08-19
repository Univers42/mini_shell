/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:44:51 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/19 14:14:27 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <unistd.h>

// Military theme colors
# define MILITARY_GREEN "\033[38;5;22m"
# define MILITARY_AMBER "\033[38;5;214m"
# define MILITARY_RED "\033[38;5;196m"
# define MATRIX_GREEN "\033[38;5;46m"

typedef enum e_render_mode
{
	RENDER_NONE = 0,
	RENDER_SIMPLE,
	RENDER_FANCY,
	RENDER_MILITARY,
	RENDER_MATRIX,
	RENDER_HACKER,
}	t_render_mode;

/* expose globals so other modules (main, metadata) can read/update status */
extern t_render_mode	g_render_mode;
extern int				g_last_status;

char	*build_prompt(void);

#endif