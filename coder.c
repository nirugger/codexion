/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 22:56:18 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 11:26:33 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


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

	while(i < 100)
	{
		log_msg(c->log_mutex, get_time() - c->start, c->coder_id, "STAMPO IO!");
		i++;
	}

}
// --- CODER'S ROUTINE -----------
// pthread_mutex_lock()
// 
// mutex_lock()
// take_dongles()
// print(time, id has taken a dongle)
// print(time, id has taken a dongle)
// print(time, id is compiling)
// sleep(compile_time)
// release_dongles()
// pthread_cond_broadcast()
// mutex_unlock()
//
// print(time, id is debugging)
// sleep(debug_time)
// print(time, id is refactoring)
// sleep(refactor_time)
// 
// check_win_condition()
// 
// 
// 
// 
// 
// 
// 
