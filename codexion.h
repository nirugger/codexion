/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nirugger <nirugger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 10:28:33 by nirugger          #+#    #+#             */
/*   Updated: 2026/05/18 18:04:37 by nirugger         ###   ########.fr       */
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
# include "ft_run.h"

/*
 * Core structures of the Codexion simulator.
 * Used to describe coders, dongles, wait queues, and global state.
 */

# define TRUE 1
# define FALSE 0

# define OK 0
# define KO 1

typedef struct s_msg	t_msg;
typedef struct s_args	t_args;
typedef struct s_queue	t_queue;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct s_sim	t_sim;
typedef struct s_names	t_names;

struct s_msg
{
	char	*dong;
	char	*comp;
	char	*dbug;
	char	*rfac;
	char	*burn;
};

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
	t_msg	msg;
	int		visual;
};

struct s_queue
{
	long	request_time;
	long	burnout_time;
};

struct s_dongle
{
	int				id;
	int				is_free;
	long			release_time;
	t_args			*args;
	t_queue			queue[2];
	pthread_mutex_t	d_mtx;
	pthread_cond_t	d_cnd;
};

struct s_coder
{
	int				id;
	char			*name;
	int				n_comp;
	long			start;
	long			last_comp;
	long			request_time;
	t_args			*args;
	t_sim			*sim;
	t_dongle		*d_min;
	t_dongle		*d_max;
	pthread_t		coding_t;
	pthread_mutex_t	c_mtx;
	pthread_mutex_t	*log_mtx;
};

struct s_names
{
	char	**ft_list;
	int		nb_names;
};

struct s_sim
{
	int				burnout;
	long			start;
	t_names			names;
	t_args			*args;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitoring;
	pthread_mutex_t	burn_mutex;
	pthread_mutex_t	log_mtx;
};

/// --- INIT & RUN -----------------------------------------------------------
int		validate_args(int argc, char **argv, t_args *args);
int		init_codexion(t_args *args, t_sim *sim);
int		run_codexion(t_sim *sim);
void	*monitor_routine(void *sim);
void	*coder_routine(void *c);

/// --- HELPS & FRIENDS ------------------------------------------------------
int		check_burnout(t_coder *coder);
int		is_first(t_dongle *d, t_coder *c);
void	update_queue_values(t_dongle *d, t_coder *c, int reset);
int		free_mutex_and_arrays(t_sim *sim, int i, int c_mtx_flag);
long	get_time(void);

/// --- VISUAL & PEERS -------------------------------------------------------
int		count_names(char **names);
void	shuffle_names(char **names);
void	log_visual(t_coder *c, char *s, long t);
char	*get_color(t_coder *c, char *msg);
char	*get_face(t_coder *c, char *msg);
char	**get_names(void);

#endif
