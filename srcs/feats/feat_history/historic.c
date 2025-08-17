/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:44:31 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/17 17:02:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"


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