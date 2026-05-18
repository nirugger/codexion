/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:32:57 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/18 01:25:25 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/// @brief Asks if flag is a valid flag.
/// @param arg : the flag to check.
/// @return 'TRUE' if it is a valid flag, 'FALSE' otherwise.
static int	is_valid_flag(char *flag)
{
	if (strcmp(flag, "--visual") != 0)
		return (FALSE);
	return (TRUE);
}

/// @brief Asks if arg is a valid positive integer.
/// @param arg : the string to be parsed.
/// @return 'TRUE' if it is a valid positive integer, 'FALSE' otherwise.
static int	is_valid_integer(char *arg)
{
	int	i;
	int	len;

	i = 0;
	len = strlen(arg);
	if (len > 11)
		return (FALSE);
	if (len == 11 && strcmp(arg, "2147483647") > 0)
		return (FALSE);
	while (i < len)
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

/// @brief Fill the t_args struct with validated arguments.
/// @param argv Arguments.
/// @param args t_args struct.
static void	fill_args(char **argv, t_args *args)
{
	args->number_of_coders = atoi(argv[1]);
	args->time_to_burnout = atoi(argv[2]);
	args->time_to_compile = atoi(argv[3]);
	args->time_to_debug = atoi(argv[4]);
	args->time_to_refactor = atoi(argv[5]);
	args->number_of_compiles_required = atoi(argv[6]);
	args->dongle_cooldown = atoi(argv[7]);
	args->scheduler = argv[8];
	args->msg.dong = "has taken a dongle";
	args->msg.burn = "burned out";
	args->msg.comp = "is compiling";
	args->msg.dbug = "is debugging";
	args->msg.rfac = "is refactoring";
}

/// @brief Validate the arguments and fill the t_args struct.
/// @param argc Number of arguments.
/// @param argv Arguments.
/// @param args t_args struct.
/// @return OK if arguments are valid, KO otherwise.
int	validate_args(int argc, char **argv, t_args *args)
{
	int	i;

	args->visual = 0;
	if (argc == 10 && is_valid_flag(argv[9]))
	{
		args->visual = 1;
		argc -= 1;
	}
	else
	{
		if (argc != 9)
			return (KO);
	}
	i = 1;
	while (i < argc - 1)
	{
		if (!is_valid_integer(argv[i]))
			return (KO);
		i++;
	}
	if (strcmp(argv[i], "fifo") && strcmp(argv[i], "edf"))
		return (KO);
	fill_args(argv, args);
	return (OK);
}
