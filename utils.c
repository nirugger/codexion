/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:08:35 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/16 03:16:28 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Prints a generic error message.
/// @return KO.
int	error(void)
{
	printf("[ERROR]\n");
	return (KO);
}

/// @brief Returns the current time in milliseconds.
/// @return Current time in ms.
long	get_time(void)
{
	struct timeval	tvanni;

	gettimeofday(&tvanni, NULL);
	return ((tvanni.tv_sec * 1000) + (tvanni.tv_usec / 1000));
}

/// @brief Frees mutexes, conds, coders and dongles when needed.
/// @param sim Shared simulation structure.
/// @param i Number of sync object to destroy.
/// @param c_mutex_flag 0 to keep coders mutexes, 1 for destroy them.
/// @return KO.
int	free_mutex_and_arrays(t_sim *sim, int i, int c_mutex_flag)
{
	int	j;

	j = i;
	if (c_mutex_flag)
	{
		j = sim->args->number_of_coders;
		while (i > 0)
		{
			i--;
			pthread_mutex_destroy(&sim->coders[i].c_mutex);
		}
	}
	while (j > 0)
	{
		j--;
		pthread_cond_destroy(&sim->dongles[j].dongle_cond);
		pthread_mutex_destroy(&sim->dongles[j].dongle_mutex);
	}
	free(sim->coders);
	free(sim->dongles);
	return (KO);
}

/// @brief Checks whether the simulation has reached a burnout state.
/// @param coder Pointer to the coder checking.
/// @return TRUE if burnout was signaled, FALSE otherwise.
int	check_burnout(t_coder *coder)
{
	int	result;

	result = FALSE;
	pthread_mutex_lock(&coder->sim->burn_mutex);
	if (coder->sim->burnout)
		result = TRUE;
	pthread_mutex_unlock(&coder->sim->burn_mutex);
	return (result);
}

/// @brief Checks whether a dongle is still in cooldown.
/// @param d Dongle to check.
/// @return TRUE if the dongle is in cooldown, FALSE otherwise.
int	in_cooldown(t_dongle *d)
{
	return (get_time() - d->release_time < d->args->dongle_cooldown);
}
