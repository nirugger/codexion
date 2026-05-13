/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:25:00 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 11:27:31 by nirugger         ###   ########.fr       */
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
	t_coder			coders[2];
	pthread_t		coder_threads[2];
	pthread_mutex_t	log_mutex;

	long			start;
	int				i;

	// if (validate_args(argc, argv, &args) != OK)
	// 	return (error());
	// printf("PARSER PASSATO");

	start = get_time();
	i = 0;
	pthread_mutex_init(&log_mutex, NULL);


	while(i < 2)
	{
		coders[i].coder_id = i + 1;
		coders[i].log_mutex = &log_mutex;
		coders[i].start = start;
		i++;
	}
	i = 0;
	while(i < 2)
	{
		pthread_create(&coder_threads[i], NULL, coder_routine, &coders[i]);
		i++;
	}
	i = 0;
	while(i < 2)
	{
		pthread_join(coder_threads[i], NULL);
		i++;
	}
}
