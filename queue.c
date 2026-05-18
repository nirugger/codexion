/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 02:08:29 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/17 22:22:58 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Determines whether a coder has priority for a shared dongle.
/// @param d Shared dongle.
/// @param c Coder requesting the dongle.
/// @return TRUE if the coder is first in queue, FALSE otherwise.
int	is_first(t_dongle *d, t_coder *c)
{
	int		this_coder;
	int		other_coder;
	long	this_time;
	long	other_time;

	if (d->id == c->id)
		this_coder = 0;
	else
		this_coder = 1;
	other_coder = 1 - this_coder;
	if (d->queue[other_coder].request_time == -1)
		return (TRUE);
	if (strcmp(c->args->scheduler, "fifo") == 0)
	{
		this_time = d->queue[this_coder].request_time;
		other_time = d->queue[other_coder].request_time;
	}
	else
	{
		this_time = d->queue[this_coder].burnout_time;
		other_time = d->queue[other_coder].burnout_time;
	}
	return (this_time <= other_time);
}

/// @brief Updates or resets the dongle queue data for a coder.
/// @param d Shared dongle.
/// @param c Associated coder.
/// @param reset If TRUE, clears the queue information.
void	update_queue_values(t_dongle *d, t_coder *c, int reset)
{
	int	index;

	if (d->id == c->id)
		index = 0;
	else
		index = 1;
	if (reset)
	{
		d->queue[index].request_time = -1;
		d->queue[index].burnout_time = -1;
		return ;
	}
	d->queue[index].request_time = get_time();
	d->queue[index].burnout_time = c->last_comp;
}
