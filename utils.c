/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:08:35 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 00:09:39 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tvanni;

	gettimeofday(&tvanni, NULL);
	return ((tvanni.tv_sec * 1000) + (tvanni.tv_usec / 1000));
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

