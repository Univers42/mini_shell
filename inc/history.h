/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:48:43 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/17 19:11:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include <stdbool.h>
# include <stddef.h>
# include <limits.h>
# include <readline/history.h>
# include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include "libft/data_structures/doubly_linked_list/ft_doubly_list.h"

# define DEFAULT_HISTFILE ".minishell_history"
# define DEFAULT_HISTSIZE 500

/* Constructor-like options */
typedef struct s_history_opts
{
	bool        persist; 	/* persist to file or keep in-session only */
	const char *histfile;	/* path or NULL for default (~/.minishell_history) */
	int         histsize;	/* max entries; <=0 means unlimited */
} t_history_opts;

/* Opaque "class" */
/**
 * One-time init; envp used for HOME and env overrides (MS_HISTORY, MS_HISTSIZE)
 * Load from file into readline and internal list (if persist enabled)
 * Add a line (dedup adjacent, obey size, mirrors into readline)
 * Save to file (no-op if persist disabled)
 * Shutdown: save (if persist), free internals, clear readline history
 * Dump history to NULL-terminated array (malloc'ed, caller frees)
 * Accessors / setters
 */
typedef struct s_history_api
{
	
	int			(*init)(const t_history_opts *opts, char **envp);
	void		(*load)(void);
	void		(*add)(const char *line);
	void		(*save)(void);
	void		(*shutdown)(void);
	char		**(*dump)(void);
	const char	*(*file)(void);          /* expanded path or NULL */
	int         (*size)(void);          /* current max size (readline stifle) */
	void        (*set_persist)(bool on);
	void        (*set_size)(int n);
}				t_history_api;

/* Get singleton vtable */
const t_history_api	*hs(void);

typedef struct s_history_state
{
	t_doubly_list	*list;
	bool			persist;
	int				histsize;
	char			histfile[PATH_MAX];
	bool			initialized;
}					t_history_state;

inline static t_history_state	*S(void)
{
	static t_history_state	*inst = NULL;

	if (!inst)
	{
		inst = (t_history_state *)calloc(1, sizeof(*inst));
		if (!inst)
			return NULL;
		inst->list = ft_dll_create();
		if (!inst->list)
			return (free(inst), NULL);
		inst->persist = true;
		inst->histsize = DEFAULT_HISTSIZE;
		inst->histfile[0] = '\0';
		inst->initialized = false;
	}
	return (inst);
}

/* Utility: expand "~" using $HOME into out buffer; returns out or NULL on error */
const char	*expand_hist_path(const char *name, char *out, size_t outsz);
void		dll_push_tail_line(const char *line);
void		dll_clear_all(void);
int			api_size(void);
void		api_set_persist(bool on);
void		api_set_size(int n);
const char	*api_file(void);
void		api_add(const char *line);
void		api_save(void);
void		api_load(void);
int			api_init(const t_history_opts *opts, char **envp);
void		api_shutdown(void);
char		**api_dump(void);

inline static const t_history_api	*hs(void)
{
	static const t_history_api api = {
	.init = api_init,
	.load = api_load,
	.add = api_add,
	.save = api_save,
	.dump = api_dump,
	.shutdown = api_shutdown,
	.file = api_file,
	.size = api_size,
	.set_persist = api_set_persist,
	.set_size = api_set_size
	};

	return (&api);
}

#endif