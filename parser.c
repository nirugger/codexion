/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:32:57 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/11 11:49:45 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	is_valid_integer(char *c)
{
	int	j;
	int len;

	j = 0;
	len = strlen(c);
	if (len > 11)
		return (BASE_FAIL);
	if (len == 11 && strcmp(c, "2147483647") > 0)
		return (BASE_FAIL);
	while (j < len)
	{
		if (c[j] < '0' || c[j] > '9')
			return (FALSE);
		j++;
	}
	return (TRUE);
}

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
}


int	parse_args(int argc, char **argv, t_args *args)
{
	int	i;

	if (argc != 9)
		return (BASE_FAIL);
	i = 1;
	while (i < argc - 1)
	{
		if (!is_valid_integer(argv[i]))
			return (BASE_FAIL);
		i++;
	}
	if (strcmp(argv[i], "fifo") && strcmp(argv[i], "edf"))
		return (BASE_FAIL);

	fill_args(argv, args);
	return (SUCCESS);
}
