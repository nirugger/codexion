/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:28:33 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/13 19:16:12 by nirugger         ###   ########.fr       */
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

# define TRUE 1
# define FALSE 0

# define OK 0
# define KO 1

typedef struct s_args	t_args;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct s_sim	t_sim;
typedef struct s_msg	t_msg;

struct s_args
{
	int		number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int		number_of_compiles_required;
	long	dongle_cooldown;
	char	*scheduler;
};

struct s_dongle
{
	int				dongle_id;
	int				is_free;
	long			release_time;
	t_args			*args;
	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	dongle_cond;
};

struct s_coder
{
	int				coder_id;
	int				total_compile;
	long			start;
	long			start_compiling;
	t_args			*args;
	t_sim			*sim;
	t_msg			*msg;
	t_dongle		*dongle_min;
	t_dongle		*dongle_max;
	pthread_mutex_t	*coder_mutex;
	pthread_mutex_t *log_mutex;
};

struct s_msg
{
	char	*dongle;
	char	*compile;
	char	*debug;
	char	*refactor;
	char	*burnout;
};

struct s_sim
{
	t_args			*args;
	t_dongle		*dongles;
	t_coder			*coders;
	int				burnout;
	long			start;
	int				end;
	pthread_mutex_t *coder_mutex;
};

int		validate_args(int argc, char **argv, t_args *args);
void	*coder_routine(void  *c);
int		error(void);
long	get_time(void);
void	take_dongle(t_dongle *dongle);
void	release_dongle(t_dongle *dongle);
int		check_burnout(t_coder *coder);
void	*sim_routine(void *sim);
void	init_msg_struct(t_msg *msgs);
int		in_cooldown(t_dongle *dongle);

#endif


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
