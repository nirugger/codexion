/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:25:00 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/11 11:23:14 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	main(int argc, char **argv)
{
	t_args	args;
	if(parse_args(argc, argv, &args) != SUCCESS)
		return error();
	else
		printf("PARSER PASSATO");
}
