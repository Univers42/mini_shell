/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

int	api_init(const t_history_opts *opts, char **envp)
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
		custom_stifle_history(st->histsize);
	else
		custom_unstifle_history();
	st->initialized = true;
	return (0);
}

void	api_load(void)
{
	t_history_state	*st;
	FILE			*f;
	char			buf[4096];
	char			*line = NULL;
	size_t			len = 0, cap = 0;

	st = S();
	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;

	f = fopen(st->histfile, "r");
	if (!f)
		return;

	/* Accumulate lines safely (handle >4k lines by chunking) */
	while (fgets(buf, sizeof(buf), f))
	{
		size_t bl = ft_strlen(buf);
		if (len + bl + 1 > cap)
		{
			size_t ncap = (cap ? cap * 2 : 8192);
			while (ncap < len + bl + 1)
				ncap *= 2;
			char *tmp = (char *)realloc(line, ncap);
			if (!tmp) { free(line); fclose(f); return; }
			line = tmp; cap = ncap;
		}
		ft_memcpy(line + len, buf, bl);
		len += bl;
		line[len] = '\0';
		if (len > 0 && line[len - 1] == '\n')
		{
			line[len - 1] = '\0';
			/* push to our DLL and to readline (allowed) */
			dll_push_tail_line(line);
			add_history(line);
			len = 0;
		}
	}
	/* last partial line without newline */
	if (len > 0 && line)
	{
		dll_push_tail_line(line);
		add_history(line);
	}
	free(line);
	fclose(f);
	update_history_length();
}

void	api_save(void)
{
	t_history_state	*st;
	FILE			*f;
	t_dll_node		*node;

	st = S();
	if (!st || !st->initialized)
		return ;
	if (!st->persist || st->histfile[0] == '\0')
		return ;

	f = fopen(st->histfile, "w");
	if (!f)
		return;

	/* Write our internal list as plain lines */
	node = st->list ? st->list->head : NULL;
	while (node)
	{
		const char *s = (const char *)node->data;
		if (s && *s)
		{
			fputs(s, f);
			fputc('\n', f);
		}
		node = node->next;
	}
	fclose(f);
}
