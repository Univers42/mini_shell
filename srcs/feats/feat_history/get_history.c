/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_history.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:59:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/17 21:59:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/**
 * history_get - Get a specific history entry by index
 * @offset: Index of the history entry (1-based, like readline)
 * 
 * Returns: Pointer to HIST_ENTRY, or NULL if index is out of range
 */
HIST_ENTRY *history_get(int offset)
{
    HIST_ENTRY **list;
    
    if (offset <= 0)
        return (NULL);
    
    list = history_list();
    if (!list || offset > history_length)
        return (NULL);
    
    return (list[offset - 1]); // Convert from 1-based to 0-based indexing
}

/**
 * current_history - Get the current history entry
 * 
 * Returns: Pointer to the last (most recent) history entry, or NULL
 */
HIST_ENTRY *current_history(void)
{
    if (history_length <= 0)
        return (NULL);
    
    return (history_get(history_length));
}

/**
 * previous_history - Get the previous history entry
 * 
 * Returns: Pointer to the second-to-last history entry, or NULL
 */
HIST_ENTRY *previous_history(void)
{
    if (history_length <= 1)
        return (NULL);
    
    return (history_get(history_length - 1));
}

/**
 * history_total_bytes - Calculate total bytes used by history
 * 
 * Returns: Total number of bytes used by all history entries
 */
int history_total_bytes(void)
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
            total += ft_strlen((char *)node->data) + 1; // +1 for null terminator
        node = node->next;
    }
    
    return (total);
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
    
    // In a full implementation, you might want to maintain
    // a current position variable in your history state
    return (1);
}
