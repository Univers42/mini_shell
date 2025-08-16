/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:44:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/16 16:47:58 by syzygy           ###   ########.fr       */
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

typedef struct s_history_state
{
	t_doubly_list	*list;
	bool			persist;
	int				histsize;
	char			histfile[PATH_MAX];
	bool			initialized;
}					t_history_state;

static t_history_state	*S(void)
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

//HELPERS

//HELPERS

const char	*expand_hist_path(const char *name, char *out, size_t outsz)
{
	const char	*home;
	size_t		len;

	if (!name || !out || outsz == 0)
		return NULL;
	home = getenv("HOME");
	if (name[0] == '~' && home && *home)
	{
		if (name[1] == '/' || name[1] == '\0')
		{
			if (ft_snprintf(out, outsz, "%s/%s", home, name + 1) <= 0)
				return (NULL);
			return (out);
		}
	}
	len = ft_strclen(name, '\0');
	if (len > outsz - 1)
		len = outsz - 1;
	ft_strlcpy(out, name, outsz);
	out[len] = '\0';
	return (out);
}

static void dll_push_tail_line(const char *line)
{
	t_history_state	*st;
	char			*dup;
	char			*last;
	char			*old;

	st = S();
	if (!st || !st->list || !line || !*line)
		return ;
	if (st->list->size > 0)
	{
		last = (char *)ft_dll_back(st->list);
		if (last && ft_strcmp(last, line) == 0)
			return ;
	}
	dup = ft_strdup(line);
	if (!dup)
		return ;
	if (!ft_dll_push_back(st->list, dup))
		free(dup);
	else if (st->histsize > 0 && st->list->size > (size_t)st->histsize)
	{
		old = (char *)ft_dll_pop_front(st->list);
		free(old) ;
	}
}

static void dll_clear_all(void)
{
	t_history_state		*st;
	char				*s;

	st = S();
	if (!st || !st->list)
		return;
	while (!ft_dll_is_empty(st->list))
	{
		s = (char *)ft_dll_pop_front(st->list);
		free(s);
	}
}

/* -------------------------- API methods ------------------------------- */

static int	api_init(const t_history_opts *opts, char **envp)
{
	t_history_state		*st;
	const char			*env_hist;
	const char			*env_size;
	long				v;

	st = S();
	(void)envp;
	if (!st)
		return (1);
	st->persist = true;
	st->histsize = DEFAULT_HISTSIZE;
	st->histfile[0] = '\0';
	env_hist = getenv("MS_HISTORY");
	env_size = getenv("MS_HISTSIZE");
	if (opts)
	{
		st->persist = opts->persist;
		if (opts->histsize != 0)
			st->histsize = opts->histsize;
		if (opts->histfile && *opts->histfile)
			expand_hist_path(opts->histfile, st->histfile, sizeof(st->histfile));
	}
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
	if (st->histsize > 0)
		stifle_history(st->histsize);
	else
		unstifle_history();
	st->initialized = true;
	return (0);
}

static void	api_load(void)
{
	t_history_state	*st;
	HIST_ENTRY		**arr; // <-- fix type: pointer to array
	int				n;
	int				i;

	st = S();
	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;
	(void)read_history(st->histfile);
	{
		arr = history_list();
		n = history_length;
		i = 0;
		while (arr && i < n)
		{
			if (arr[i] && arr[i]->line)
				dll_push_tail_line(arr[i]->line);
			i++;
		}
	}
}

static void api_add(const char *line)
{
	t_history_state	*st;
	const	char *p;

	st = S();
	if (!st || !st->initialized || !line || !*line)
		return;
	{
		p = line;
		while (*p && ft_isspace((unsigned char)*p))
			p++;
		if (*p == '\0')
			return ;
	}
	dll_push_tail_line(line);
	add_history(line);
	if (st->histsize > 0)
		stifle_history(st->histsize);
}

static void	api_save(void)
{
	t_history_state	*st;

	st = S();
	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;
	(void)write_history(st->histfile);
}

static void	api_shutdown(void)
{
	t_history_state	*st;

	st = S();
	if (!st)
		return ;
	api_save();
	dll_clear_all();
	rl_clear_history();
	if (st->list)
		ft_dll_destroy(st->list);
	ft_memset(st, 0, sizeof(*st));
}

static const char *api_file(void)
{
	t_history_state *st;

	st = S();
	if (!st)
		return (NULL);
	if (st->histfile[0])
		return (st->histfile);
	return (NULL);
}

static int api_size(void)
{
	t_history_state *st = S();

	st = S();
	if (st)
		return (st->histsize);
	return (0);
}

static void api_set_persist(bool on)
{
	t_history_state	*st;

	st = S();
	if (!st)
		return ;
	st->persist = on;
}

static void api_set_size(int n)
{
	t_history_state *st;

	st = S();
	if (!st)
		return;
	st->histsize = n;
	if (n > 0)
		stifle_history(n);
	else
		unstifle_history();
}

static char **api_dump(void)
{
	t_history_state	*st;
	size_t			n;
	size_t			i;
	char			**out;
	t_dll_node		*node;

	st = S();
	if (!st || !st->list)
		return (NULL);
	n = st->list->size;
	out = (char **)malloc((n + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	i = 0;
	node = st->list->head;
	while (i < n && node)
	{
		if (node->data)
		{
			out[i] = ft_strdup((const char *)node->data);
			if (!out[i])
			{
				while (i > 0)
					free(out[--i]);
				free(out);
				return (NULL);
			}
		}
		else
			out[i] = NULL;
		i++;
		node = node->next;
	}
	out[n] = NULL;
	return (out);
}

/* -------------------------- VTable export ----------------------------- */
/**temporary global variable just for the prototype history api
then we will be working on refactorization */
static const t_history_api G_API =
{
	.init        = api_init,
	.load        = api_load,
	.add         = api_add,
	.save        = api_save,
	.shutdown    = api_shutdown,
	.dump        = api_dump,
	.file        = api_file,
	.size        = api_size,
	.set_persist = api_set_persist,
	.set_size    = api_set_size
};

const t_history_api *hs(void)
{
	return (&G_API);
}