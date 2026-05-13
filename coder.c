/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:56:18 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 19:27:22 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_coder *coder)
{
	return (coder->sim->burnout);
}

static void log_msg(pthread_mutex_t *log_mutex, long t, int id, char *msg)
{
	pthread_mutex_lock(log_mutex);
	printf("%ld ms, coder %d %s\n", t, id, msg);
	pthread_mutex_unlock(log_mutex);
	return ;
}




void	*coder_routine(void  *coder)
{
	t_coder	*c = (t_coder *)coder;
	int i = 0;
	struct timespec ts;
	long wake_ms;

	// log_msg(c->log_mutex, get_time() - c->sim->start, c->coder_id, "SONO PARTITO\n");

	while(c->total_compile < c->args->number_of_compiles_required)
	{

		pthread_mutex_lock(&c->dongle_min->dongle_mutex);
		while (in_cooldown(c->dongle_min) || c->dongle_min->is_free == FALSE)
		{
			// write(1, "waiting\n", 8);
			wake_ms = c->dongle_min->release_time + c->args->dongle_cooldown;
			ts.tv_sec = wake_ms / 1000;
			ts.tv_nsec = (wake_ms % 1000) * 1000000L;
			// usleep(10);
			// pthread_cond_broadcast(&c->dongle_min->dongle_cond);
			// pthread_cond_broadcast(&c->dongle_max->dongle_cond);
			pthread_cond_timedwait(&c->dongle_min->dongle_cond, &c->dongle_min->dongle_mutex, &ts);
			// write(1, "finito di aspettare\n", 20);
		}
		// else
			// pthread_cond_wait(&c->dongle_min->dongle_cond, &c->dongle_min->dongle_mutex);

		// 
		pthread_mutex_unlock(&c->dongle_min->dongle_mutex);
		take_dongle(c->dongle_min);
		log_msg(c->log_mutex, get_time() - c->start, c->coder_id, c->msg->dongle);
		// check_burnout(coder);


		pthread_mutex_lock(&c->dongle_max->dongle_mutex);
		while (in_cooldown(c->dongle_max) || c->dongle_max->is_free == FALSE)
		{
			// write(1, "waiting\n", 8);
			wake_ms = c->dongle_max->release_time + c->args->dongle_cooldown;
			ts.tv_sec = wake_ms / 1000;
			ts.tv_nsec = (wake_ms % 1000) * 1000000L;
			// usleep(10);
			// pthread_cond_broadcast(&c->dongle_max->dongle_cond);
			// pthread_cond_broadcast(&c->dongle_max->dongle_cond);
			pthread_cond_timedwait(&c->dongle_max->dongle_cond, &c->dongle_max->dongle_mutex, &ts);
			// write(1, "finito di aspettare\n", 20);
		}
		// else
			// pthread_cond_wait(&c->dongle_max->dongle_cond, &c->dongle_max->dongle_mutex);

		// 
		pthread_mutex_unlock(&c->dongle_max->dongle_mutex);
		take_dongle(c->dongle_max);
		log_msg(c->log_mutex, get_time() - c->start, c->coder_id, c->msg->dongle);

		
		log_msg(c->log_mutex, get_time() - c->start, c->coder_id, c->msg->compile);
		usleep(c->args->time_to_compile);
		// pthread_mutex_unlock(&c->dongle_min->dongle_mutex);
		// ho finito di compilare, lo rilascio
		release_dongle(c->dongle_min);

		log_msg(c->log_mutex, get_time() - c->start, c->coder_id, c->msg->debug);
		usleep(c->args->time_to_debug);

		log_msg(c->log_mutex, get_time() - c->start, c->coder_id, c->msg->refactor);
		usleep(c->args->time_to_refactor);
		c->total_compile++;
	}
	log_msg(c->log_mutex, get_time() - c->sim->start, c->coder_id, "SONO USCITO\n");
}
