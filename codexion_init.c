/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 02:24:15 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/15 04:17:38 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Allocates arrays for coders and dongles.
/// @param sim Shared simulation structure.
/// @return OK if allocation succeeded, KO otherwise.
static int	init_arrays(t_sim *sim)
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

/// @brief Initializes the state of each dongle and its sync objects.
/// @param sim Shared simulation structure.
/// @return OK if initialization succeeded, KO otherwise.
static int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].args = sim->args;
		sim->dongles[i].is_free = TRUE;
		sim->dongles[i].release_time = get_time() - sim->args->dongle_cooldown;
		sim->dongles[i].queue[0].request_time = -1;
		sim->dongles[i].queue[0].burnout_time = -1;
		sim->dongles[i].queue[1].request_time = -1;
		sim->dongles[i].queue[1].burnout_time = -1;
		if (pthread_cond_init(&sim->dongles[i].dongle_cond, NULL) != OK)
			break ;
		if (pthread_mutex_init(&sim->dongles[i].dongle_mutex, NULL) != OK)
		{
			pthread_cond_destroy(&sim->dongles[i].dongle_cond);
			break ;
		}
		i++;
	}
	if (i != sim->args->number_of_coders)
		return (free_mutex_and_arrays(sim, i, 0));
	return (OK);
}

/// @brief Initializes coders and their mutexes.
/// @param sim Shared simulation structure.
/// @return OK if initialization succeeded, KO otherwise.
static int	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].log_mutex = &sim->log_mutex;
		sim->coders[i].n_comp = 0;
		sim->coders[i].args = sim->args;
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].c_mutex, NULL) != OK)
			return (free_mutex_and_arrays(sim, i, 1));
		i++;
	}
	return (OK);
}

/// @brief Assigns dongles to each coder thanks to Dijkstra <3
/// @param sim Shared simulation structure.
static void	assign_dongles(t_sim *sim)
{
	int	i;
	int	dongle_min;
	int	dongle_max;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		dongle_min = i;
		dongle_max = (i + 1) % sim->args->number_of_coders;
		if (dongle_min < dongle_max)
		{
			sim->coders[i].d_min = &sim->dongles[dongle_min];
			sim->coders[i].d_max = &sim->dongles[dongle_max];
		}
		else
		{
			sim->coders[i].d_min = &sim->dongles[dongle_max];
			sim->coders[i].d_max = &sim->dongles[dongle_min];
		}
		i++;
	}
}

/// @brief Initializes the simulation, allocating and synchronizing.
/// @param args Parameters passed from the command line.
/// @param sim Shared simulation structure.
/// @return OK if initialization completed successfully, KO otherwise.
int	init_codexion(t_args *args, t_sim *sim)
{
	sim->args = args;
	if (init_arrays(sim) != OK)
		return (KO);
	sim->burnout = FALSE;
	if (init_dongles(sim) != OK)
		return (KO);
	if (init_coders(sim) != OK)
		return (KO);
	assign_dongles(sim);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != OK)
		return (free_mutex_and_arrays(sim, sim->args->number_of_coders, 1));
	if (pthread_mutex_init(&sim->burn_mutex, NULL) != OK)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		return (free_mutex_and_arrays(sim, sim->args->number_of_coders, 1));
	}
	return (OK);
}
