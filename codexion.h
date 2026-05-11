/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:28:33 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/11 18:26:29 by nirugger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define FALSE 0
# define TRUE 1

# define SUCCESS 0
# define BASE_FAIL 1

typedef struct s_coder	t_coder;
typedef struct s_sim	t_sim;

typedef struct s_args
{
	int		number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int		number_of_compiles_required;
	long	dongle_cooldown;
	char	*scheduler;
}	t_args;

typedef struct s_dongle
{
	int				dongle_id;
	long			release_time;
	t_args			*args;
	pthread_mutex_t	d_mutex;
}	t_dongle;

//	pthread_mutex_init()
//	pthread_mutex_lock()
//		sezione critica
//	pthread_cond_broadcast()
//	pthread_mutex_unlock()

// while (pthread_cond_wait(&cond))
// {
// 	continue;
// };
// pthread_mutex_lock;

struct s_coder
{
	int				coder_id;
	long			start_burning_out;
	long			start_compiling;
	long			start_refactoring;
	pthread_mutex_t	c_mutex;
	t_args			*args;
};

int	parse_args(int argc, char **argv, t_args *args);
int	error(void);

#endif