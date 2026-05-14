/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:19:48 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 23:25:16 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	burn_msg(t_coder *c)
{
	long	t;
	int		id;

	id = c->id;
	t = get_time() - c->start;
	pthread_mutex_lock(&c->sim->log_mutex);
	printf("%ld ms -> coder %d %s\n", t, id, c->args->msg.burn);
	pthread_mutex_unlock(&c->sim->log_mutex);
}

int	has_burned_out(t_sim *sim, int i)
{
	pthread_mutex_lock(&sim->coders[i].c_mutex);
	if (get_time() - sim->coders[i].burning >= sim->args->time_to_burnout)
	{
		pthread_mutex_lock(&sim->burn_mutex);
		sim->burnout = 1;
		pthread_mutex_unlock(&sim->burn_mutex);
		pthread_mutex_unlock(&sim->coders[i].c_mutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&sim->coders[i].c_mutex);
	return (FALSE);
}


static int	has_finished(t_coder *c)
{
	pthread_mutex_lock(&c->c_mutex);
	if(c->n_comp >= c->args->number_of_compiles_required)
	{
		pthread_mutex_unlock(&c->c_mutex);
		return (TRUE);
	}
	pthread_mutex_unlock(&c->c_mutex);
	return (FALSE);
}

void	*monitor_routine(void *sim)
{
	t_sim	*s;
	int		i;
	int		finished;
	int		exit;

	s = (t_sim *)sim;
	finished = 0;
	exit = 0;
	while (finished < s->args->number_of_coders)
	{
		i = 0;
		finished = 0;
		while (i < s->args->number_of_coders)
		{
			if (has_finished(&s->coders[i]))
			{
				i++;
				finished++;
				continue ;
			}
			exit = has_burned_out(s, i);
			if (exit)
			{
				burn_msg(&s->coders[i]);
				return (NULL);
			}
			i++;
		}
		usleep(10);
	}
}
