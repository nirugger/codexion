/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 00:32:10 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/15 12:28:37 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Prints a synchronized message if the simulation is still active.
/// @param log_mutex Shared mutex for printing.
/// @param c Coder that is logging.
/// @param msg Message to print.
/// @return OK if the message was printed, KO if burnout has already occurred.
static int	log_msg(pthread_mutex_t *log_mutex, t_coder *c, char *msg)
{
	int		id;
	long	t;

	id = c->id;
	t = get_time() - c->start;
	pthread_mutex_lock(log_mutex);
	if (check_burnout(c))
	{
		pthread_mutex_unlock(log_mutex);
		return (KO);
	}
	printf("%ld ms -> coder %d %s\n", t, id, msg);
	pthread_mutex_unlock(log_mutex);
	return (OK);
}

/// @brief Attempts to acquire a dongle respecting cooldown and priority.
/// @param d Dongle to acquire.
/// @param c Coder requesting the dongle.
static void	take_dongle(t_dongle *d, t_coder *c)
{
	long			wake_ms;
	struct timespec	ts;

	pthread_mutex_lock(&d->dongle_mutex);
	update_queue_values(d, c, FALSE);
	while (get_time() - d->release_time < d->args->dongle_cooldown
		|| d->is_free == FALSE || !is_first(d, c))
	{
		if (check_burnout(c) == TRUE)
		{
			update_queue_values(d, c, 1);
			pthread_mutex_unlock(&d->dongle_mutex);
			return ;
		}
		wake_ms = d->release_time + d->args->dongle_cooldown;
		ts.tv_sec = wake_ms / 1000;
		ts.tv_nsec = (wake_ms % 1000) * 1000000L;
		pthread_cond_timedwait(&d->dongle_cond, &d->dongle_mutex, &ts);
	}
	update_queue_values(d, c, TRUE);
	d->is_free = FALSE;
	pthread_mutex_unlock(&d->dongle_mutex);
}

/// @brief Releases a dongle and updates its cooldown timer.
///	pthread_cond_broadcast(&dongle->dongle_cond);
/// @param dongle Dongle to release.
static void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_free = TRUE;
	dongle->release_time = get_time();
	pthread_mutex_unlock(&dongle->dongle_mutex);
}

/// @brief Performs the compilation cycle for a coder.
/// @param c Coder that compiles.
/// @return OK if compilation succeeded, KO if the simulation ended.
static int	compile(t_coder *c)
{
	take_dongle(c->d_min, c);
	if (log_msg(c->log_mutex, c, c->args->msg.dong) != OK)
		return (KO);
	take_dongle(c->d_max, c);
	if (log_msg(c->log_mutex, c, c->args->msg.dong) != OK)
		return (KO);
	if (log_msg(c->log_mutex, c, c->args->msg.comp) != OK)
		return (KO);
	pthread_mutex_lock(&c->c_mutex);
	c->burning = get_time();
	c->n_comp++;
	pthread_mutex_unlock(&c->c_mutex);
	usleep(c->args->time_to_compile * 1000);
	release_dongle(c->d_min);
	release_dongle(c->d_max);
	return (OK);
}

/// @brief Main routine of the coder thread.
/// @param coder Pointer to the t_coder structure.
/// @return NULL at the end of the routine.
void	*coder_routine(void *coder)
{
	t_coder	*c;

	c = (t_coder *)coder;
	if (c->id % 2)
		usleep(50);
	while (c->n_comp < c->args->number_of_compiles_required)
	{
		if (compile(c) != OK)
			break ;
		if (log_msg(c->log_mutex, c, c->args->msg.dbug) != OK)
			break ;
		usleep(c->args->time_to_debug * 1000);
		if (log_msg(c->log_mutex, c, c->args->msg.rfac) != OK)
			break ;
		usleep(c->args->time_to_refactor * 1000);
	}
	return (NULL);
}
