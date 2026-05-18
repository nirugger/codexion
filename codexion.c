/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:24:38 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/17 22:22:58 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Sets the start time for the simulation coders.
/// @param sim Shared simulation structure.
static void	set_start(t_sim *sim)
{
	int	i;

	i = 0;
	sim->start = get_time();
	while (i < sim->args->number_of_coders)
	{
		sim->coders[i].start = sim->start;
		sim->coders[i].last_comp = sim->start;
		i++;
	}
	return ;
}

/// @brief Creates the monitor thread and one thread for each coder.
/// @param sim Shared simulation structure.
/// @return number of coder threads created or KO on error.
static int	create_threads(t_sim *sim)
{
	int	i;

	i = 0;
	if (pthread_create(&sim->monitoring,
			NULL, monitor_routine, sim) != OK)
		return (KO);
	while (i < sim->args->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].coding_t,
				NULL, coder_routine, &sim->coders[i]) != OK)
			break ;
		i++;
	}
	return (i);
}

/// @brief Waits for threads to finish and joins them.
/// @param sim Shared simulation structure.
/// @param i Number of coder threads actually created.
/// @return OK if all threads were created before join, KO otherwise.
static int	join_threads(t_sim *sim, int i)
{
	int	ret;

	if (i != sim->args->number_of_coders)
		ret = (KO);
	else
		ret = (OK);
	while (i > 0)
	{
		i--;
		pthread_join(sim->coders[i].coding_t, NULL);
	}
	pthread_join(sim->monitoring, NULL);
	return (ret);
}

/// @brief Starts the simulation, waits for completion, and frees resources.
/// @param sim Shared simulation structure.
/// @return OK if the simulation ends correctly, KO on error.
int	run_codexion(t_sim *sim)
{
	int	index;
	int	result;

	set_start(sim);
	index = create_threads(sim);
	result = join_threads(sim, index);
	free_mutex_and_arrays(sim, sim->args->number_of_coders, 1);
	pthread_mutex_destroy(&sim->burn_mutex);
	pthread_mutex_destroy(&sim->log_mtx);
	return (result);
}
