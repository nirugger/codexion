/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:25:00 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/15 03:22:41 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Entry point of the Codexion simulator.
/// @param argc Number of command line arguments.
/// @param argv Array of argument strings.
/// @return OK if the simulation completed successfully, KO otherwise.
int	main(int argc, char **argv)
{
	t_args			args;
	t_sim			sim;

	if (validate_args(argc, argv, &args) != OK)
		return (error());
	if (init_codexion(&args, &sim) != OK)
		return (error());
	if (run_codexion(&sim) != OK)
		return (error());
	return (OK);
}
