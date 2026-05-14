/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:08:35 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 19:23:29 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tvanni;

	gettimeofday(&tvanni, NULL);
	return ((tvanni.tv_sec * 1000) + (tvanni.tv_usec / 1000));
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
		pthread_cond_destroy(&sim->dongles[j].dongle_cond);
		pthread_mutex_destroy(&sim->dongles[j].dongle_mutex);
	}
	free(sim->coders);
	free(sim->dongles);
	return (KO);
}

// void	assign_mutex(t_sim *sim)
// {
// 	pthread_mutex_init(&sim->log_mutex, NULL);

// }
// void	init_msg_struct(t_msg *msg)
// {
// 	msg->dongle = "has taken a dongle";
// 	msg->burnout = "burned out";
// 	msg->compile = "is compiling";
// 	msg->debug = "is debugging";
// 	msg->refactor = "is refactoring";
// }

