/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:40:36 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 15:58:32 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	in_cooldown(t_dongle *d)
{
	return (get_time() - d->release_time < d->args->dongle_cooldown);
};

void	take_dongle(t_dongle *d, t_coder *c)
{
	long			wake_ms;
	struct timespec	ts;

	pthread_mutex_lock(&d->dongle_mutex);
	while (in_cooldown(d) || d->is_free == FALSE)
	{
		if (check_burnout(c) == TRUE)
		{
			pthread_mutex_unlock(&d->dongle_mutex);
			return ;
		}
		wake_ms = d->release_time + d->args->dongle_cooldown;
		ts.tv_sec = wake_ms / 1000;
		ts.tv_nsec = (wake_ms % 1000) * 1000000L;
		pthread_cond_timedwait(&d->dongle_cond, &d->dongle_mutex, &ts);
	}
	d->is_free = FALSE;
	pthread_mutex_unlock(&d->dongle_mutex);
};

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_free = TRUE;
	dongle->release_time = get_time();
	pthread_cond_broadcast(&dongle->dongle_cond);
	pthread_mutex_unlock(&dongle->dongle_mutex);
};

void	assign_dongles(t_sim *sim)
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

int	cleaup_and_return(t_sim *sim, int i, int c_mutex_flag)
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
		pthread_cond_destroy(&sim->dongles[i].dongle_cond);
		pthread_mutex_destroy(&sim->dongles[i].dongle_mutex);
	}
	free(sim->coders);
	free(sim->dongles);
	return (KO);
}


int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].args = sim->args;
		sim->dongles[i].is_free = TRUE;
		sim->dongles[i].release_time = get_time() - sim->args->dongle_cooldown;
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
		return (cleaup_and_return(sim, i, 0));
	return (OK);
}
