/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:40:36 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 18:02:34 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	in_cooldown(t_dongle *dongle)
{
	// printf("time: %ld\n", get_time() - dongle->release_time);
	// // if (get_time() - dongle->release_time >= dongle->args->dongle_cooldown)
	// pthread_cond_broadcast(&dongle->dongle_cond);
	return (get_time() - dongle->release_time < dongle->args->dongle_cooldown);
};

void	take_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	// pthread_cond_broadcast(&dongle->dongle_cond);
	dongle->is_free = FALSE;
	pthread_mutex_unlock(&dongle->dongle_mutex);
};

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_free = TRUE;
	dongle->release_time = get_time();
	pthread_mutex_unlock(&dongle->dongle_mutex);
	pthread_cond_broadcast(&dongle->dongle_cond);
};
