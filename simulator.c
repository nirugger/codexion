/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:24:38 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 17:08:17 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	check_cooldown(t_dongle *dongle)
{
	// pthread_mutex_lock(&dongle->dongle_mutex);
	if (get_time() - dongle->release_time >= dongle->args->dongle_cooldown)
	{
		// pthread_mutex_unlock(&dongle->dongle_mutex);
		pthread_cond_broadcast(&dongle->dongle_cond);
	}
	// else
		// pthread_mutex_unlock(&dongle->dongle_mutex);

	// return ;
}


void	*sim_routine(void *sim)
{
	t_sim *s = (t_sim *)sim;
	int d = 0;
	while(1)
	{
		usleep(100);
		d = 0;
		while (d < 2)
		{
			check_cooldown(&s->dongles[d]);
			d++;
		}
	}
}