/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:19:48 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 15:53:04 by nirugger         ###   ########.fr       */
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

		printf("%ld\n", get_time() - sim->coders[i].burning);
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
	int		exit;

	s = (t_sim *)sim;
	exit = 0;
	while (1)
	{
		i = 0;
		while (i < s->args->number_of_coders)
		{
			if (has_finished(&s->coders[i]))
			{
				i++;
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
		// printf("SONO IN UN LOOP\n");
	}
}
