/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:24:38 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 03:10:56 by nirugger         ###   ########.fr       */
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


// void	*sim_routine(void *sim)
// {
// 	t_sim *s = (t_sim *)sim;
// 	int d = 0;
// 	while(1)
// 	{
// 		usleep(100);
// 		d = 0;
// 		while (d < 2)
// 		{
// 			check_cooldown(&s->dongles[d]);
// 			d++;
// 		}
// 	}
// }

int	init_arrays(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->args->number_of_coders);
	if (!sim->coders)
		return (KO);
	sim->dongles = malloc(sizeof(t_dongle) * sim->args->number_of_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		return (KO);
	}
	return (OK);
}


int	init_simulation(t_args *args, t_sim *sim)
{
	sim->args = args;
	if (init_arrays(sim) != OK)
		return (KO);
	sim->start = get_time();
	sim->burnout = FALSE;
	sim->done = 0;
	if (init_dongles(sim) != OK)
		return (KO);
	init_coders(sim);
	assign_dongles(sim);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != OK)
		return (cleaup_and_return(sim, sim->args->number_of_coders));
	if (pthread_mutex_init(&sim->burn_mutex, NULL) != OK)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		return (cleaup_and_return(sim, sim->args->number_of_coders));
	}
	return (OK);
}
