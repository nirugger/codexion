/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:25:00 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/18 01:40:18 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Prints a generic error message.
/// @return KO.
static int	error(char *type, char *flag)
{
	if (strcmp(type, "base") == 0)
		printf("[ERROR]\n");
	else if (strcmp(type, "flag") == 0)
	{
		printf("%s[ERROR]:%s unrecognised argument ", RED, RESET);
		printf("'%s'. ", flag);
		printf("Did you mean '--visual'?\n");
	}
	return (KO);
}

/// @brief Entry point of the Codexion simulator.
/// @param argc Number of command line arguments.
/// @param argv Array of argument strings.
/// @return OK if the simulation completed successfully, KO otherwise.
int	main(int argc, char **argv)
{
	t_args			args;
	t_sim			sim;

	if (validate_args(argc, argv, &args) != OK && argc == 10)
		return (error("flag", argv[9]));
	if (validate_args(argc, argv, &args) != OK && argc != 10)
		return (error("base", argv[9]));
	if (init_codexion(&args, &sim) != OK)
		return (error("base", argv[9]));
	if (run_codexion(&sim) != OK)
		return (error("base", argv[9]));
	write(1, "\n", 1);
	if (sim.burnout == TRUE)
		return (KO);
	return (OK);
}
