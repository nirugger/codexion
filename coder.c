/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:56:18 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 20:52:19 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->burn_mutex);
	if (coder->sim->burnout)
	{
		pthread_mutex_unlock(&coder->sim->burn_mutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&coder->sim->burn_mutex);
	return (FALSE);
}

int	log_msg(pthread_mutex_t *log_mutex, t_coder *c, char *msg)
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

void	*coder_routine(void  *coder)
{
	t_coder	*c;

	c = (t_coder *)coder;
	if (c->id % 2)
		usleep(50);
	while(c->n_comp < c->args->number_of_compiles_required)
	{
		take_dongle(c->d_min, c);
		if (log_msg(c->log_mutex, c, c->args->msg.dong) != OK)
		break ;
		take_dongle(c->d_max, c);
		if (log_msg(c->log_mutex, c, c->args->msg.dong) != OK)
			break ;
		if (log_msg(c->log_mutex, c, c->args->msg.comp) != OK)
			break ;
		pthread_mutex_lock(&c->c_mutex);
		c->burning = get_time();
		c->n_comp++;
		pthread_mutex_unlock(&c->c_mutex);
		usleep(c->args->time_to_compile * 1000);

		release_dongle(c->d_min);
		release_dongle(c->d_max);
		if (log_msg(c->log_mutex, c, c->args->msg.dbug) != OK)
			break ;
		usleep(c->args->time_to_debug * 1000);
		if (log_msg(c->log_mutex, c, c->args->msg.rfac) != OK)
			break ;
		usleep(c->args->time_to_refactor * 1000);
	}
	return (NULL);
}

int	init_coders(t_sim *sim)
{
	int i;

	i = 0;
	while(i < sim->args->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].log_mutex = &sim->log_mutex;
		sim->coders[i].burning = get_time();
		sim->coders[i].start = sim->start;
		sim->coders[i].n_comp = 0;
		sim->coders[i].args = sim->args;
		sim->coders[i].sim = sim;
		if(pthread_mutex_init(&sim->coders[i].c_mutex, NULL) != OK)
			return (cleaup_and_return(sim, i, 1));
		i++;
	}
	return (OK);
}
