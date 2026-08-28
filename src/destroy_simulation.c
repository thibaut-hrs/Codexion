/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_simulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:21:13 by thours            #+#    #+#             */
/*   Updated: 2026/08/28 22:32:16 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	destroy_simulation(t_simulation *simulation)
{
	if (simulation->state_mutex_initialized)
	{
		pthread_mutex_destroy(&simulation->state_mutex);
		simulation->state_mutex_initialized = 0;
	}
	if (simulation->dongles)
		destroy_dongles(simulation);
	if (simulation->coders)
	{
		free(simulation->coders);
		simulation->coders = NULL;
	}
}

void	destroy_dongles(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->dongles_initialized)
	{
		if (simulation->dongles[i].cond_initialized)
		{
			pthread_cond_destroy(&simulation->dongles[i].cond);
			simulation->dongles[i].cond_initialized = 0;
		}
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		i++;
	}
	free(simulation->dongles);
	simulation->dongles = NULL;
	simulation->dongles_initialized = 0;
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