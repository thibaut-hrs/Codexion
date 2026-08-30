/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 08:55:52 by thours            #+#    #+#             */
/*   Updated: 2026/08/30 13:04:48 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	init_simulation(t_simulation *simulation, t_config config)
{
	simulation->config = config;
	simulation->coders = NULL;
	simulation->dongles = NULL;
	simulation->monitor_created = 0;
	simulation->dongles_initialized = 0;
	simulation->start_time = get_time_ms();
	simulation->threads_created = 0;
	simulation->finished = 0;
	simulation->next_request_order = 0;
	if (pthread_mutex_init(&simulation->state_mutex, NULL) != 0)
		return (simulation->state_mutex_initialized = 0, 0);
	simulation->state_mutex_initialized = 1;
	if (pthread_cond_init(&simulation->state_cond, NULL) != 0)
		return (simulation->state_cond_initialized = 0, 0);
	simulation->state_cond_initialized = 1;
	if (!init_dongles(simulation))
	{
		destroy_simulation(simulation);
		return (0);
	}
	if (!init_coders(simulation))
	{
		destroy_simulation(simulation);
		return (0);
	}
	if (!queue_init(&simulation->queue, simulation->config.scheduler))
	{
		simulation->queue_initialized = 0;
		destroy_simulation(simulation);
		return (0);
	}
	simulation->queue_initialized = 1;
	return (1);
}

int	init_dongles(t_simulation *simulation)
{
	int	i;

	simulation->dongles = malloc(
			sizeof(t_dongle) * simulation->config.number_of_coders
			);
	if (!simulation->dongles)
		return (0);
	simulation->dongles_initialized = 0;
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		simulation->dongles[i].id = i + 1;
		simulation->dongles[i].state = DONGLE_FREE;
		simulation->dongles_initialized++;
		i++;
	}
	return (1);
}

int	init_coders(t_simulation *simulation)
{
	int	i;

	simulation->coders = malloc(
			sizeof(t_coder) * simulation->config.number_of_coders
			);
	if (!simulation->coders)
		return (0);
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		simulation->coders[i].id = i + 1;
		simulation->coders[i].state = STATE_DEBUGGING;
		simulation->coders[i].left_dongle = &simulation->dongles[i];
		simulation->coders[i].right_dongle = &simulation->dongles[
			(i + 1) % simulation->config.number_of_coders
		];
		simulation->coders[i].last_compile_start = simulation->start_time;
		simulation->coders[i].compile_count = 0;
		simulation->coders[i].simulation = simulation;
		i++;
	}
	return (1);
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
