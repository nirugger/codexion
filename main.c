/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:25:00 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/11 13:39:59 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	long			time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	main(int argc, char **argv)
{
	t_args	args;

	if (parse_args(argc, argv, &args) != SUCCESS)
		return (error());
	else
		printf("PARSER PASSATO");
}
