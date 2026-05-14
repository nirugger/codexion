/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:40:36 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 23:24:30 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	in_cooldown(t_dongle *d)
{
	return (get_time() - d->release_time < d->args->dongle_cooldown);
};

int is_first(t_dongle *d, t_coder *c)
{
	int		this_coder;
	int		other_coder;
	long	this_time;
	long	other_time;

	if (d->id == c->id)
		this_coder = 0;
	else
		this_coder = 1;
	other_coder = 1 - this_coder;
	if (d->queue[other_coder].request_time == -1)
		return (TRUE);
	if (strcmp(c->args->scheduler, "fifo") == 0)
	{
		this_time = d->queue[this_coder].request_time;
		other_time = d->queue[other_coder].request_time;
	}
	else
	{
		this_time = d->queue[this_coder].burnout_time;
		other_time = d->queue[other_coder].burnout_time;
	}
	return (this_time <= other_time);
}

void	update_queue_values(t_dongle *d, t_coder *c, int reset)
{
	int	index;

	if (d->id == c->id)
		index = 0;
	else
		index = 1;

	if (reset)
	{
		d->queue[index].request_time = -1;
		d->queue[index].burnout_time = -1;
		return;
	}
	d->queue[index].request_time = get_time();
	d->queue[index].burnout_time = c->burning;
}

void	take_dongle(t_dongle *d, t_coder *c)
{
	int				i;
	long			wake_ms;
	struct timespec	ts;

	pthread_mutex_lock(&d->dongle_mutex);
	update_queue_values(d, c, 0);
	while (in_cooldown(d) || d->is_free == FALSE || !is_first(d, c))
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
	update_queue_values(d, c, 1);
	d->is_free = FALSE;
	pthread_mutex_unlock(&d->dongle_mutex);
};

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_free = TRUE;
	dongle->release_time = get_time();
	// pthread_cond_broadcast(&dongle->dongle_cond);
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
		sim->dongles[i].queue[0].request_time = -1;
		sim->dongles[i].queue[0].burnout_time = -1;
		sim->dongles[i].queue[1].request_time = -1;
		sim->dongles[i].queue[1].burnout_time = -1;
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
