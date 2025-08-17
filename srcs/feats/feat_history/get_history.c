/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_history.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:59:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 23:45:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

// Convert from 1-based to 0-based indexing [REPAIR THE ERROR]
HIST_ENTRY *custom_history_get(int offset)
{
    HIST_ENTRY **list;

    if (offset <= 0)
        return (NULL);

    list = custom_history_list();
    if (!list || offset > custom_history_length)
        return (NULL);

    return (list[offset - 1]);
}

HIST_ENTRY *custom_current_history(void)
{
    if (custom_history_length <= 0)
        return (NULL);
    return (custom_history_get(custom_history_length));
}

HIST_ENTRY *custom_previous_history(void)
{
    if (custom_history_length <= 1)
        return (NULL);
    return (custom_history_get(custom_history_length - 1));
}

int custom_history_total_bytes(void)
{
    t_history_state *st;
    t_dll_node *node;
    int total = 0;

    st = S();
    if (!st || !st->list)
        return (0);
    node = st->list->head;
    while (node)
    {
        if (node->data)
            total += ft_strlen((char *)node->data) + 1;
        node = node->next;
    }
    return (total);
}

int custom_where_history(void)
{
    return (custom_history_length > 0 ? custom_history_length - 1 : 0);
}

int custom_history_set_pos(int pos)
{
    if (pos < 0 || pos >= custom_history_length)
        return (0);
    return (1);
}

/**
 * where_history - Get current position in history (always returns last)
 * 
 * Returns: Current position (0-based index of last entry)
 */
int where_history(void)
{
    return (history_length > 0 ? history_length - 1 : 0);
}

/**
 * history_set_pos - Set current position in history
 * @pos: Position to set (0-based)
 * 
 * Note: This is a simplified implementation that doesn't maintain
 *       internal position state like readline does.
 * 
 * Returns: 1 on success, 0 on failure
 */
int history_set_pos(int pos)
{
    if (pos < 0 || pos >= history_length)
        return (0);
    return (1);
}
