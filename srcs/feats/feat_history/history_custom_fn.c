/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_custom_fn.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 23:40:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/08/19 14:04:37 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/* Our own authoritative history length (do not clash with libreadline) */
int g_custom_history_length = 0;

/* forward declaration for the static accessor */
static t_history_list_state *get_history_list_state(void);

t_hist_entry **custom_history_list(void)
{
    t_history_state *st;
    t_history_list_state *state;
    t_hist_entry **hist_array;
    size_t *array_size;
    t_dll_node *node;
    size_t i;

    st = S();
    if (!st || !st->list || st->list->size == 0)
    {
        g_custom_history_length = 0;
        return (NULL);
    }

    state = get_history_list_state();
    hist_array = *(state->hist_array_ptr);
    array_size = state->array_size_ptr;

    /* Resize array if needed (add 1 for NULL terminator) */
    if (*array_size < st->list->size + 1)
    {
        if (hist_array)
        {
            for (i = 0; i < *array_size; i++)
            {
                if (hist_array[i])
                {
                    free(hist_array[i]);
                    hist_array[i] = NULL;
                }
            }
            free(hist_array);
        }

        *array_size = st->list->size + 1;
        hist_array = (t_hist_entry **)malloc(*array_size * sizeof(t_hist_entry *));
        if (!hist_array)
        {
            *array_size = 0;
            g_custom_history_length = 0;
            return (NULL);
        }
        for (i = 0; i < *array_size; i++)
            hist_array[i] = NULL;

        *(state->hist_array_ptr) = hist_array;
    }

    /* Fill the array with history entries */
    i = 0;
    node = st->list->head;
    while (node && i < st->list->size)
    {
        if (!hist_array[i])
        {
            hist_array[i] = (t_hist_entry *)malloc(sizeof(t_hist_entry));
            if (!hist_array[i])
                break;
        }

        hist_array[i]->line = (char *)node->data;
        hist_array[i]->timestamp = NULL;
        hist_array[i]->data = NULL;

        node = node->next;
        i++;
    }

    /* NULL terminate the array */
    if (i < *array_size)
        hist_array[i] = NULL;

    g_custom_history_length = (int)i;
    return (hist_array);
}

static t_history_list_state *get_history_list_state(void)
{
    static t_hist_entry **hist_array = NULL;
    static size_t array_size = 0;
    static t_history_list_state state = { &hist_array, &array_size };

    return (&state);
}

void cleanup_history_list(void)
{
    t_history_list_state *state;
    size_t i;

    state = get_history_list_state();

    if (*(state->hist_array_ptr))
    {
        for (i = 0; i < *(state->array_size_ptr); i++)
        {
            if ((*(state->hist_array_ptr))[i])
            {
                free((*(state->hist_array_ptr))[i]);
                (*(state->hist_array_ptr))[i] = NULL;
            }
        }
        free(*(state->hist_array_ptr));
        *(state->hist_array_ptr) = NULL;
        *(state->array_size_ptr) = 0;
    }
    g_custom_history_length = 0;
}

void update_history_length(void)
{
    t_history_state *st;

    st = S();
    if (st && st->list)
        g_custom_history_length = (int)st->list->size;
    else
        g_custom_history_length = 0;
}