/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:24:38 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/15 00:15:40 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


static int	create_threads(t_sim *sim)
{
	int	i;

	i = 0;
	if (pthread_create(&sim->monitoring,
		NULL, monitor_routine, &sim) != OK)
		return (KO);
	while(i < sim->args->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].coding,
			NULL, coder_routine, &sim->coders[i]) != OK)
			break ;
		i++;
	}
	return (i);
}

static int	join_threads(t_sim *sim, int i)
{
	int	ret;

	if (i != sim->args->number_of_coders)
		ret = (KO);
	else
		ret = (OK);
	while(i > 0)
	{
		i--;
		pthread_join(sim->coders[i].coding, NULL);
	}
	pthread_join(sim->monitoring, NULL);
	return (ret);
}


int	run_simulation(t_sim *sim)
{
	int	index;
	int	result;

	index = create_threads(sim);
	result = join_threads(sim, index);
	cleaup_and_return(sim, sim->args->number_of_coders, 1);
	pthread_mutex_destroy(&sim->burn_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
	return (result);
}

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
	// sim->done = 0;

	if (init_dongles(sim) != OK)
		return (KO);
	if (init_coders(sim) != OK)
		return (KO);
	assign_dongles(sim);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != OK)
		return (cleaup_and_return(sim, sim->args->number_of_coders, 1));
	if (pthread_mutex_init(&sim->burn_mutex, NULL) != OK)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		return (cleaup_and_return(sim, sim->args->number_of_coders, 1));
	}
	return (OK);
}
