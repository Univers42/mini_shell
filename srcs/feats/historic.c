/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:44:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/16 15:53:34 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "history.h"

/* --------------------- Internal singleton "class" ---------------------- */

typedef struct s_history_state {
	t_doubly_list *list;
	bool           persist;
	int            histsize;
	char           histfile[PATH_MAX];
	bool           initialized;
} t_history_state;

static t_history_state *S(void)
{
	static t_history_state *inst = NULL;

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
	return inst;
}

/* ------------------------ Small utilities ----------------------------- */

const char *expand_hist_path(const char *name, char *out, size_t outsz)
{
	const char *home;
	size_t      len;

	if (!name || !out || outsz == 0)
		return NULL;
	home = getenv("HOME");
	if (name[0] == '~' && home && *home)
	{
		if (name[1] == '/' || name[1] == '\0')
		{
			if (ft_snprintf(out, outsz, "%s/%s", home, name + 1) <= 0)
				return NULL;
			return out;
		}
	}
	len = ft_strclen(name, '\0');
	if (len > outsz - 1)
		len = outsz - 1;
	ft_strlcpy(out, name, outsz);
	out[len] = '\0';
	return out;
}

static void dll_push_tail_line(const char *line)
{
	t_history_state *st = S();
	char *dup;

	if (!st || !st->list || !line || !*line)
		return;
	/* adjacent dedup like bash HISTCONTROL=ignoredups */
	if (st->list->size > 0)
	{
		char *last = (char *)ft_dll_back(st->list);
		if (last && ft_strcmp(last, line) == 0)
			return;
	}
	dup = ft_strdup(line);
	if (!dup)
		return;
	if (!ft_dll_push_back(st->list, dup))
		free(dup);
	else if (st->histsize > 0 && st->list->size > (size_t)st->histsize)
	{
		char *old = (char *)ft_dll_pop_front(st->list);
		free(old);
	}
}

static void dll_clear_all(void)
{
	t_history_state *st = S();
	char *s;

	if (!st || !st->list)
		return;
	while (!ft_dll_is_empty(st->list))
	{
		s = (char *)ft_dll_pop_front(st->list);
		free(s);
	}
}

/* -------------------------- API methods ------------------------------- */

static int api_init(const t_history_opts *opts, char **envp)
{
	t_history_state *st = S();
	const char *env_hist;
	const char *env_size;
	long       v;

	(void)envp;
	if (!st)
		return 1;

	/* defaults */
	st->persist = true;
	st->histsize = DEFAULT_HISTSIZE;
	st->histfile[0] = '\0';

	/* env overrides */
	env_hist = getenv("MS_HISTORY");
	env_size = getenv("MS_HISTSIZE");

	/* apply opts first (if any) */
	if (opts)
	{
		st->persist = opts->persist;
		if (opts->histsize != 0)
			st->histsize = opts->histsize;
		if (opts->histfile && *opts->histfile)
			expand_hist_path(opts->histfile, st->histfile, sizeof(st->histfile));
	}

	/* environment can override file and size if present */
	if (env_hist && *env_hist)
		expand_hist_path(env_hist, st->histfile, sizeof(st->histfile));
	if (st->histfile[0] == '\0')
		expand_hist_path("~/" DEFAULT_HISTFILE, st->histfile, sizeof(st->histfile));

	if (env_size && *env_size)
	{
		v = ft_atol(env_size);
		if (v > 0 && v <= INT_MAX)
			st->histsize = (int)v;
	}

	/* configure readline cap */
	if (st->histsize > 0)
		stifle_history(st->histsize);
	else
		unstifle_history();

	st->initialized = true;
	return 0;
}

static void api_load(void)
{
	t_history_state *st = S();

	if (!st || !st->initialized)
		return;
	if (!st->persist || st->histfile[0] == '\0')
		return;

	/* load into readline */
	(void)read_history(st->histfile);

	/* mirror into our dll */
	{
		HIST_ENTRY **arr = history_list();
		int         n = history_length;
		int         i;

		for (i = 0; arr && i < n; ++i)
		{
			if (arr[i] && arr[i]->line)
				dll_push_tail_line(arr[i]->line);
		}
	}
}

static void api_add(const char *line)
{
	t_history_state *st = S();

	if (!st || !st->initialized || !line || !*line)
		return;
	/* ignore pure whitespace lines (bash-like) */
	{
		const char *p = line;
		while (*p && ft_isspace((unsigned char)*p)) p++;
		if (*p == '\0')
			return;
	}

	/* adjacent dedup + dll */
	dll_push_tail_line(line);

	/* readline side (dedup adjacent on our own already) */
	add_history(line);
	if (st->histsize > 0)
		stifle_history(st->histsize);
}

static void api_save(void)
{
	t_history_state *st = S();

	if (!st || !st->initialized)
		return;
	if (!st->persist || st->histfile[0] == '\0')
		return;
	(void)write_history(st->histfile);
}

static void api_shutdown(void)
{
	t_history_state *st = S();

	if (!st)
		return;
	/* persist before freeing */
	api_save();

	/* clear in-memory and readline histories */
	dll_clear_all();
	rl_clear_history();

	/* free singleton */
	if (st->list)
		ft_dll_destroy(st->list);
	memset(st, 0, sizeof(*st));
	/* keep the static storage; next S() will re-alloc */
}

static const char *api_file(void)
{
	t_history_state *st = S();
	if (!st)
		return NULL;
	return st->histfile[0] ? st->histfile : NULL;
}

static int api_size(void)
{
	t_history_state *st = S();
	return st ? st->histsize : 0;
}

static void api_set_persist(bool on)
{
	t_history_state *st = S();
	if (!st) return;
	st->persist = on;
}

static void api_set_size(int n)
{
	t_history_state *st = S();
	if (!st) return;
	st->histsize = n;
	if (n > 0)
		stifle_history(n);
	else
		unstifle_history();
}

/* -------------------------- VTable export ----------------------------- */

static const t_history_api G_API = {
	.init        = api_init,
	.load        = api_load,
	.add         = api_add,
	.save        = api_save,
	.shutdown    = api_shutdown,
	.file        = api_file,
	.size        = api_size,
	.set_persist = api_set_persist,
	.set_size    = api_set_size
};

const t_history_api *hs(void)
{
	return &G_API;
}