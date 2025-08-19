/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:16:14 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/19 14:08:17 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include <stdbool.h>
# include <stddef.h>
# include <limits.h>
# include <readline/history.h>
# include "libft.h"
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define DEFAULT_HISTFILE	".minishell_history"
# define DEFAULT_HISTSIZE	500
# define PERSISTENCE		true

/* Constructor-like options */
typedef struct s_history_opts
{
	bool		persist;	// persist to file or keep in-session only 
	const char	*histfile;	// path or NULL for default (~/.minishell_history)
	int			histsize;	// size of file
}				t_history_opts;

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
	const char	*(*file)(void);	// expanded path or NULL
	int			(*size)(void);	// current max size (readline stifle)
	void		(*set_persist)(bool on);
	void		(*set_size)(int n);
}				t_history_api;

typedef struct s_history_state
{
	t_doubly_list	*list;
	bool			persist;
	int				histsize;
	char			histfile[PATH_MAX];
	bool			initialized;
}					t_history_state;

/* Global singleton accessor (one definition in historic.c) */
/* Private, non-conflicting history accessors
(avoid clashing with libreadline) */
extern int					g_custom_history_length;
typedef struct _hist_entry	t_hist_entry;

typedef struct s_history_list_state
{
	t_hist_entry	***hist_array_ptr;
	size_t			*array_size_ptr;
}					t_history_list_state;

const t_history_api	*hs(void);
t_history_state		*S(void);
const char			*expand_hist_path(const char *name,
						char *out, size_t outsz);
void				dll_push_tail_line(const char *line);
void				dll_clear_all(void);
int					api_size(void);
void				api_set_persist(bool on);
void				api_set_size(int n);
const char			*api_file(void);
void				api_add(const char *line);
void				api_save(void);
void				api_load(void);
int					api_init(const t_history_opts *opts, char **envp);
void				api_shutdown(void);
char				**api_dump(void);
int					custom_stifle_history(int max);
int					custom_unstifle_history(void);
void				cleanup_history_list(void);
void				update_history_length(void);
t_hist_entry		**custom_history_list(void);
t_hist_entry		*custom_history_get(int offset);
t_hist_entry		*custom_current_history(void);
t_hist_entry		*custom_previous_history(void);
int					custom_history_total_bytes(void);
int					custom_where_history(void);
int					custom_history_set_pos(int pos);
#endif