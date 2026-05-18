/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 02:40:16 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/18 18:02:07 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

char	**get_names(void)
{
	static char	*s_names[] = {
		S00, S01, S02, S03, S04, S05, S06, S07, S08, S09,
		S10, S11, S12, S13, S14, S15, S16, S17, S18, S19,
		S20, S21, S22, S23, S24, S25, S26, S27, S28, S29,
		S30, S31, S32, S33, S34, S35, S36, S37, S38, S39,
		S40, S41, S42, S43, S44, S45, S46, S47, S48, S49,
		S50, S51, S52, S53, S54, S55, S56, S57, S58, S59,
		NULL
	};

	return (s_names);
}

static unsigned int	ft_rand(unsigned int *seed)
{
	*seed = *seed * 1337420 + 40469;
	return (*seed);
}

int	count_names(char **names)
{
	int	i;

	i = 0;
	while (names[i])
		i++;
	return (i);
}

void	shuffle_names(char **names)
{
	struct timeval	tv;
	unsigned int	seed;
	int				i;
	int				j;
	char			*tmp;

	gettimeofday(&tv, NULL);
	seed = (unsigned int)tv.tv_usec;
	i = count_names(names);
	while (i > 1)
	{
		j = ft_rand(&seed) % i;
		i--;
		tmp = names[i];
		names[i] = names[j];
		names[j] = tmp;
	}
}

void	log_visual(t_coder *c, char *s, long t)
{
	int		nb_coders;
	char	*rgb;
	char	*ex;
	char	*n;
	int		id;

	rgb = get_color(c, s);
	ex = get_face(c, s);
	n = c->name;
	id = c->id;
	nb_coders = c->args->number_of_coders;
	if (nb_coders <= c->sim->names.nb_names)
		printf("  %s%s  %*ld → %s %s%s\n", rgb, ex, 7, t, n, s, RESET);
	else
		printf("  %s%s  %*ld → %*d %s%s\n", rgb, ex, 7, t, 3, id, s, RESET);
	return ;
}
