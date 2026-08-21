/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 10:17:34 by thours            #+#    #+#             */
/*   Updated: 2026/08/21 14:54:36 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!get_simulation_finished(coder->simulation))
	{
		coder_debug(coder);
		if (get_simulation_finished(coder->simulation))
			break ;
		coder_refactor(coder);
		if (get_simulation_finished(coder->simulation))
			break ;
		coder_compile(coder);
	}
	return (NULL);
}

void	coder_debug(t_coder *coder)
{
	coder->state = STATE_DEBUGGING;
	printf("%lld %d is debugging\n",
		get_time_ms() - coder->simulation->start_time, coder->id);
	usleep(coder->simulation->config.time_to_debug * 1000);
}

void	coder_refactor(t_coder *coder)
{
	coder->state = STATE_REFACTORING;
	printf("%lld %d is refactoring\n",
		get_time_ms() - coder->simulation->start_time, coder->id);
	usleep(coder->simulation->config.time_to_refactor * 1000);
}

void	coder_compile(t_coder *coder)
{
	coder->state = STATE_COMPILING;
	set_last_compile_start(coder, get_time_ms());
	printf("%lld %d is compiling\n",
		get_time_ms() - coder->simulation->start_time, coder->id);
	usleep(coder->simulation->config.time_to_compile * 1000);
	increment_compile_count(coder);
}

void	set_last_compile_start(t_coder *coder, long long time)
{
	pthread_mutex_lock(&coder->simulation->state_mutex);
    coder->last_compile_start = time;
    pthread_mutex_unlock(&coder->simulation->state_mutex);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->simulation->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
}

int	start_simulation(t_simulation *simulation)
{
	int	i;

	if (pthread_create(&simulation->monitor_thread,
		NULL, monitor_routine, simulation) != 0)
	return (0);
	simulation->monitor_created = 1;
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (pthread_create(
				&simulation->coders[i].thread,
				NULL,
				coder_routine,
				&simulation->coders[i]) != 0)
			return (0);
		simulation->threads_created++;
		i++;
	}
	return (1);
}

int	join_threads(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->threads_created)
	{
		if (pthread_join(simulation->coders[i].thread, NULL) != 0)
			return (0);
		i++;
	}
	if (simulation->monitor_created)
	{
		if (pthread_join(simulation->monitor_thread, NULL) != 0)
			return (0);
	}
	return (1);
}