/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:19:48 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/18 01:25:11 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Prints the burnout message for a coder.
/// @param c Pointer to the coder.
static void	burn_msg(t_coder *c)
{
	long	t;
	int		id;
	char	*msg;

	id = c->id;
	msg = c->args->msg.burn;
	t = get_time() - c->start;
	pthread_mutex_lock(&c->sim->log_mtx);
	if (c->sim->args->visual)
		printf("%s%s  %*ld -> %*d %s%s\n", RED, BRN, 7, t, 3, id, msg, RESET);
	else
		printf("%*ld %*d %s\n", 5, t, 3, id, msg);
	pthread_mutex_unlock(&c->sim->log_mtx);
}

/// @brief Checks if a coder has reached burnout time.
/// @param sim Shared simulation structure.
/// @param i Index of the coder.
/// @return TRUE if the coder has burned out, FALSE otherwise.
static int	has_burned_out(t_sim *sim, int i)
{
	pthread_mutex_lock(&sim->coders[i].c_mtx);
	if (get_time() - sim->coders[i].last_comp >= sim->args->time_to_burnout)
	{
		pthread_mutex_lock(&sim->burn_mutex);
		sim->burnout = 1;
		pthread_mutex_unlock(&sim->burn_mutex);
		pthread_mutex_unlock(&sim->coders[i].c_mtx);
		return (TRUE);
	}
	pthread_mutex_unlock(&sim->coders[i].c_mtx);
	return (FALSE);
}

/// @brief Checks if a single coder has completed all required compilations.
/// @param c Pointer to the coder.
/// @return TRUE if the coder is finished, FALSE otherwise.
static int	has_finished(t_coder *c)
{
	int	result;

	result = FALSE;
	pthread_mutex_lock(&c->c_mtx);
	if (c->n_comp >= c->args->number_of_compiles_required)
		result = TRUE;
	pthread_mutex_unlock(&c->c_mtx);
	return (result);
}

/// @brief Performs periodic burnout and termination checks.
/// @param sim Shared simulation structure.
/// @param counter Count of completed coders.
/// @return Updated count of finished coders, or -1 if burnout occurs.
static int	monitor_checks(t_sim *sim, int counter)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		if (has_finished(&sim->coders[i]))
		{
			i++;
			counter++;
			continue ;
		}
		if (has_burned_out(sim, i))
		{
			burn_msg(&sim->coders[i]);
			return (-1);
		}
		i++;
	}
	return (counter);
}

/// @brief Monitoring thread routine for coders.
/// @param sim Shared simulation structure.
/// @return NULL when the monitor exits.
void	*monitor_routine(void *sim)
{
	int		coders_out;
	t_sim	*s;

	coders_out = 0;
	s = (t_sim *)sim;
	while (coders_out < s->args->number_of_coders)
	{
		if (check_burnout(s->coders))
			break ;
		coders_out = 0;
		coders_out = monitor_checks(s, coders_out);
		if (coders_out == -1)
			return (NULL);
		usleep(10);
	}
	return (NULL);
}
