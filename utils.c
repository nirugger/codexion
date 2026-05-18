/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:08:35 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/17 23:42:32 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
/// @param c_mtx_flag 0 to keep coders mutexes, 1 for destroy them.
/// @return KO.
int	free_mutex_and_arrays(t_sim *sim, int i, int c_mtx_flag)
{
	int	j;

	j = i;
	if (c_mtx_flag)
	{
		j = sim->args->number_of_coders;
		while (i > 0)
		{
			i--;
			pthread_mutex_destroy(&sim->coders[i].c_mtx);
		}
	}
	while (j > 0)
	{
		j--;
		pthread_cond_destroy(&sim->dongles[j].d_cnd);
		pthread_mutex_destroy(&sim->dongles[j].d_mtx);
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

/// @brief Gets colour prefixes for simulation logs.
/// @param c Coder needed for sim struct access.
/// @param msg The log deciding which color is returned.
/// @param color The original natural color.
/// @return The prefix that gives the color.
char	*get_color(t_coder *c, char *msg)
{
	char	*color;

	color = RESET;
	if (strcmp(msg, c->args->msg.dong) == 0)
		color = YELLOW;
	else if (strcmp(msg, c->args->msg.burn) == 0)
		color = RED;
	else if (strcmp(msg, c->args->msg.comp) == 0)
		color = GREEN;
	else if (strcmp(msg, c->args->msg.dbug) == 0)
		color = MAGENTA;
	else if (strcmp(msg, c->args->msg.rfac) == 0)
		color = CYAN;
	return (color);
}

char	*get_face(t_coder *c, char *msg)
{
	char	*face;

	face = "";
	if (strcmp(msg, c->args->msg.dong) == 0)
		face = DNG;
	else if (strcmp(msg, c->args->msg.burn) == 0)
		face = BRN;
	else if (strcmp(msg, c->args->msg.comp) == 0)
		face = CMP;
	else if (strcmp(msg, c->args->msg.dbug) == 0)
		face = DBG;
	else if (strcmp(msg, c->args->msg.rfac) == 0)
		face = RFC;
	return (face);
}
