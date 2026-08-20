/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:50:48 by thours            #+#    #+#             */
/*   Updated: 2026/08/20 19:41:40 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>

typedef struct s_simulation	t_simulation;

typedef enum e_state
{
	STATE_COMPILING,
	STATE_DEBUGGING,
	STATE_REFACTORING
}	t_state;

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

typedef enum e_dongle_state
{
	DONGLE_FREE,
	DONGLE_HELD,
	DONGLE_COOLDOWN
}	t_dongle_state;

typedef struct s_config
{
	int		    number_of_coders;
	long long	time_to_burnout;
	long long	time_to_compile;
	long long	time_to_debug;
	long long	time_to_refactor;
	int		    number_of_compiles_required;
	long long	dongle_cooldown;
	t_scheduler scheduler;
}	t_config;

typedef struct s_dongle
{
	int				id;
	t_dongle_state	state;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_state			state;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long long		last_compile_start;
	int				compile_count;
    t_simulation	*simulation;
}	t_coder;

typedef struct s_simulation
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		start_time;
}	t_simulation;

int	parse_args(int argc, char **argv, t_config *config);
int	parse_positive_ll(char *str, long long *value);
int	parse_positive_int(char *str, int *value);
int	parse_scheduler(char *str, t_scheduler *scheduler);

#endif