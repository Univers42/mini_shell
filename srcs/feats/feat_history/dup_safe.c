/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_safe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:53:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/20 15:36:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * When we call a function like `api_dump`, it uses those `dup*` func
 * to allocate and fill a new array of strings, each beig a copy of a history
 * entry.
 * this allows us to safely return  history data to other parts of the shell
 * or user code, without exposing internal pointers or risking memory
 * corruption
 * the  dup function are here to ensure safe, independant copies of history
 * data for external use, keeping the internal history list protected
 * and consistent
 */

/**
 * duplicates a string pointer (usuallly from a node)
 * it does not check for any structure it returns a duplicate 
 * or ann empty string
*/
static char	*dup_node_str(const char *s)
{
	if (s)
		return (ft_strdup(s));
	return (ft_strdup(""));
}

char	**dup_list_to_array(t_dll_node *node, size_t m)
{
	char	**out;
	size_t	j;

	j = 0;
	out = (char **)malloc((m + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	while (j < m && node)
	{
		out[j] = dup_node_str((const char *)node->data);
		if (!out[j])
			return (free_partial(out, j), NULL);
		j++;
		node = node->next;
	}
	out[m] = NULL;
	return (out);
}

/**
 * works on struct and always returns a valid string
 * -> never NULL, even if the enntry is missing.
 */
static char	*dup_line_or_empty(const t_hist_entry *e)
{
	if (e && e->line)
		return (ft_strdup(e->line));
	return (ft_strdup(""));
}

char	**dup_history_entries(t_hist_entry **harr, int n)
{
	char	**out;
	int		i;

	i = 0;
	out = (char **)malloc(((size_t)n + 1u) * sizeof(char *));
	if (!out)
		return (NULL);
	while (i < n)
	{
		out[i] = dup_line_or_empty(harr[i]);
		if (!out[i])
			return (free_partial(out, (size_t)i), NULL);
		i++;
	}
	out[n] = NULL;
	return (out);
}

int	is_duplicate_last(const char *line)
{
	int				n;
	t_hist_entry	*last;
	t_history_state	*st;

	st = access_hist_state();
	if (st)
		n = st->hist_length;
	else
		n = 0;
	if (n <= 0)
		return (0);
	last = ft_history_get(n);
	if (last && last->line && ft_strcmp(last->line, line) == 0)
		return (1);
	return (0);
}
