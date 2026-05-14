/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:19:48 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/14 01:29:24 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	monitor_routine(void *sim)
{
	t_sim	*s = (t_sim *)sim;
	int		i;

	while (s->done < s->args->number_of_coders)
	{
		i = 0;
		while (i < s->args->number_of_coders)
		{
			
		}
	}
}