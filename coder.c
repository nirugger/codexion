/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:56:18 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 02:39:24 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_coder *coder)
{
	return (coder->sim->burnout);
}

void	log_msg(pthread_mutex_t *log_mutex, long t, int id, char *msg)
{
	long	current_t;

	current_t = get_time();
	t = current_t - t;
	pthread_mutex_lock(log_mutex);
	printf("%ld ms -> coder %d %s\n", t, id, msg);
	pthread_mutex_unlock(log_mutex);
	return ;
}

void	*coder_routine(void  *coder)
{
	t_coder	*c = (t_coder *)coder;

	while(c->total_compile < c->args->number_of_compiles_required)
	{
		take_dongle(c->dongle_min, c);
		take_dongle(c->dongle_max, c);
		log_msg(c->log_mutex, c->start, c->coder_id, c->args->msg.comp);
		c->start_compiling = get_time();
		usleep(c->args->time_to_compile * 1000);
		release_dongle(c->dongle_min);
		release_dongle(c->dongle_max);
		log_msg(c->log_mutex, c->start, c->coder_id, c->args->msg.dbug);
		usleep(c->args->time_to_debug * 1000);
		log_msg(c->log_mutex, c->start, c->coder_id, c->args->msg.rfac);
		usleep(c->args->time_to_refactor * 1000);
		c->total_compile++;
	}
	log_msg(c->log_mutex, c->sim->start, c->coder_id, "SONO USCITO\n");
	return (NULL);
}

void	init_coders(t_sim *sim)
{
	int i;

	i = 0;
	while(i < sim->args->number_of_coders)
	{
		sim->coders[i].coder_id = i + 1;
		sim->coders[i].log_mutex = &sim->log_mutex;
		sim->coders[i].start = sim->start;
		sim->coders[i].total_compile = 0;
		sim->coders[i].args = sim->args;
		sim->coders[i].sim = sim;
		sim->coders[i].burn_mutex = &sim->burn_mutex;
		i++;
	}
}