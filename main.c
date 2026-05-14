/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:25:00 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 16:10:44 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	main(int argc, char **argv)
{
	t_args			args;
	t_sim			sim;
	int				i;

	if (validate_args(argc, argv, &args) != OK)
		return (error());
	if (init_simulation(&args, &sim) != OK)
		return (error());

	// TODO: da proteggere
	pthread_create(&sim.monitoring, NULL, monitor_routine, &sim);

	i = -1;
	while(++i < args.number_of_coders)
		pthread_create(&sim.coders[i].coding, NULL, coder_routine, &sim.coders[i]);
	
	// pthread_join(sim_thread, NULL);
	while(--i > -1)
		pthread_join(sim.coders[i].coding, NULL);
	pthread_join(sim.monitoring, NULL);
	
	// free_palestine
	cleaup_and_return(&sim, args.number_of_coders, 1);
	pthread_mutex_destroy(&sim.burn_mutex);
	pthread_mutex_destroy(&sim.log_mutex);
	return (OK);
}
