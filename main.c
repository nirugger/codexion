/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:25:00 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 20:10:43 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	long			time;
	struct timeval	tvanni;

	gettimeofday(&tvanni, NULL);
	return ((tvanni.tv_sec * 1000) + (tvanni.tv_usec / 1000));
}

int	main(int argc, char **argv)
{
	t_args			args;
	t_coder			coders[3];
	t_dongle		dongles[3];
	t_msg			msg;
	pthread_t		coder_threads[3];
	pthread_mutex_t	log_mutex;

	long			start;
	int				i;

	t_sim			simulator;

	if (validate_args(argc, argv, &args) != OK)
		return (error());
	printf("PARSER PASSATO\n");
	start = get_time();
	i = 0;
	pthread_mutex_init(&log_mutex, NULL);

	init_msg_struct(&msg);

	simulator.args = &args;
	simulator.coders = coders;
	simulator.dongles = dongles;
	simulator.start = start;
	simulator.end = 0;
	simulator.burnout = 0;

	while(i < 3)
	{
		dongles[i].dongle_id = i + 1;
		dongles[i].args = &args;
		dongles[i].is_free = TRUE;
		dongles[i].release_time = get_time() - args.dongle_cooldown;
		pthread_cond_init(&dongles[i].dongle_cond, NULL);
		pthread_mutex_init(&dongles[i].dongle_mutex, NULL);
		i++;
	}
	
	i = 0;
	while(i < 3)
	{
		coders[i].coder_id = i + 1;
		coders[i].log_mutex = &log_mutex;
		coders[i].start = start;
		coders[i].total_compile = 0;
		coders[i].args = &args;
		coders[i].msg = &msg;
		coders[i].sim = &simulator;
		
		i++;
	}

	coders[0].dongle_min = &dongles[0];
	coders[0].dongle_max = &dongles[1];
	coders[1].dongle_min = &dongles[1];
	coders[1].dongle_max = &dongles[2];
	coders[2].dongle_min = &dongles[0];
	coders[2].dongle_max = &dongles[2];

	// pthread_t sim_thread;
	// pthread_create(&sim_thread, NULL, sim_routine, &simulator);

	i = 0;
	while(i < 3)
	{
		pthread_create(&coder_threads[i], NULL, coder_routine, &coders[i]);
		i++;
	}
	
	// pthread_join(sim_thread, NULL);
	i = 0;
	while(i < 3)
	{
		pthread_join(coder_threads[i], NULL);
		i++;
	}
}
