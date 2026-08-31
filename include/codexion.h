/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:50:48 by thours            #+#    #+#             */
/*   Updated: 2026/08/31 13:30:04 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/time.h>
# include <unistd.h>

# define INITIAL_QUEUE_CAPACITY 16

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
	int			number_of_coders;
	long long	time_to_burnout;
	long long	time_to_compile;
	long long	time_to_debug;
	long long	time_to_refactor;
	int			number_of_compiles_required;
	long long	dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_dongle
{
	int				id;
	t_dongle_state	state;
	long long		cooldown_until;
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

typedef struct s_request
{
	t_coder			*coder;
	int				order;
	long long		deadline;
}	t_request;

typedef struct s_priority_queue
{
	t_request		*requests;
	int				size;
	int				capacity;
	t_scheduler		scheduler;
}	t_priority_queue;

typedef struct s_simulation
{
	t_config			config;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			monitor_thread;
	int					monitor_created;
	pthread_cond_t		state_cond;
	int					state_cond_initialized;
	pthread_mutex_t		state_mutex;
	int					state_mutex_initialized;
	int					finished;
	int					threads_created;
	int					dongles_initialized;
	long long			start_time;
	t_priority_queue	queue;
	int					queue_initialized;
	long long			next_request_order;
	pthread_mutex_t		log_mutex;
	int					log_mutex_initialized;
}	t_simulation;

/**** Parsing ****/
int			parse_args(int argc, char **argv, t_config *config);
int			parse_positive_ll(char *str, long long *value);
int			parse_positive_int(char *str, int *value);
int			parse_scheduler(char *str, t_scheduler *scheduler);

/**** Initialization ****/
int			init_dongles(t_simulation *simulation);
int			init_coders(t_simulation *simulation);
int			init_simulation(t_simulation *simulation, t_config config);
int			start_simulation(t_simulation *simulation);

/**** Destroy simulation ****/
void		destroy_simulation(t_simulation *simulation);
void		destroy_dongles(t_simulation *simulation);
int			join_threads(t_simulation *simulation);

/**** Coders ****/
void		*coder_routine(void *arg);
void		coder_debug(t_coder *coder);
void		coder_refactor(t_coder *coder);
void		coder_compile(t_coder *coder, t_priority_queue *queue);

/**** Coders utils ****/
void		set_last_compile_start(t_coder *coder, long long time);
void		increment_compile_count(t_coder *coder);
void		log_event(t_coder *coder, const char *message);
void		log_event_dongle(t_coder *coder, t_dongle *dongle);

/**** Compilation ****/
int			coder_can_compile(t_coder *coder, t_priority_queue *queue);
int			try_start_compile(t_coder *coder, t_priority_queue *queue);
int			create_compile_request(t_coder *coder, t_priority_queue *queue);

/**** Simulation monitoring ****/
void		*monitor_routine(void *arg);
int			coder_has_burned_out(t_coder *coder);
int			all_coders_finished(t_simulation *simulation);
void		set_simulation_finished(t_simulation *simulation, int value);
void		update_dongle_cooldown(t_dongle *dongle, t_simulation *simulation);

/**** Utils ****/
long long	get_time_ms(void);
int			get_simulation_finished(t_simulation *simulation);
long long	get_last_compile_start(t_coder *coder);
int			get_compile_count(t_coder *coder);

/**** Scheduler ****/
int			request_has_priority(t_priority_queue *queue, t_request *a,
				t_request *b);

/**** Queue operations ****/
int			queue_init(t_priority_queue *queue, t_scheduler scheduler);
int			queue_grow(t_priority_queue *queue);
int			queue_push(t_priority_queue *queue, t_request request);
int			queue_pop(t_priority_queue *queue);
void		queue_destroy(t_priority_queue *queue);

/**** Heap operations ****/
void		heap_swap(t_request *a, t_request *b);
void		heap_up(t_priority_queue *queue, int index);
void		heap_down(t_priority_queue *queue, int index);

#endif
