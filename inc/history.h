/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:48:43 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/16 15:50:25 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include <stdbool.h>
# include <stddef.h>
# include <limits.h>
# include <readline/history.h>
# include "libft.h"
# include "libft/data_structures/doubly_linked_list/ft_doubly_list.h"

# define DEFAULT_HISTFILE ".minishell_history"
# define DEFAULT_HISTSIZE 500

/* Constructor-like options */
typedef struct s_history_opts {
	bool        persist;                /* persist to file or keep in-session only */
	const char *histfile;               /* path or NULL for default (~/.minishell_history) */
	int         histsize;               /* max entries; <=0 means unlimited */
} t_history_opts;

/* Opaque "class" */
typedef struct s_history_api {
	/* One-time init; envp used for HOME and env overrides (MS_HISTORY, MS_HISTSIZE) */
	int   (*init)(const t_history_opts *opts, char **envp);
	/* Load from file into readline and internal list (if persist enabled) */
	void  (*load)(void);
	/* Add a line (dedup adjacent, obey size, mirrors into readline) */
	void  (*add)(const char *line);
	/* Save to file (no-op if persist disabled) */
	void  (*save)(void);
	/* Shutdown: save (if persist), free internals, clear readline history */
	void  (*shutdown)(void);

	/* Accessors / setters */
	const char *(*file)(void);          /* expanded path or NULL */
	int         (*size)(void);          /* current max size (readline stifle) */
	void        (*set_persist)(bool on);
	void        (*set_size)(int n);
} t_history_api;

/* Get singleton vtable */
const t_history_api *hs(void);

/* Utility: expand "~" using $HOME into out buffer; returns out or NULL on error */
const char *expand_hist_path(const char *name, char *out, size_t outsz);

#endif /* HISTORY_H */