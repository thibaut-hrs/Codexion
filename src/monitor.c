/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:01:55 by thours            #+#    #+#             */
/*   Updated: 2026/08/30 13:12:43 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	*monitor_routine(void *arg)
{
	t_simulation	*simulation;
	int				i;

	simulation = (t_simulation *)arg;
	while (!get_simulation_finished(simulation))
	{
		i = 0;
		while (i < simulation->config.number_of_coders)
		{
			if (coder_has_burned_out(&simulation->coders[i]))
			{
				set_simulation_finished(simulation, 1);
				printf("%lld %d burned out\n",
					get_time_ms() - simulation->start_time,
					simulation->coders[i].id);
				return (NULL);
			}
			i++;
		}
		update_dongle_cooldown(simulation->dongles, simulation);
		if (all_coders_finished(simulation))
		{
			return (set_simulation_finished(simulation, 1), NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	coder_has_burned_out(t_coder *coder)
{
	long long	now;
	long long	last_compile_start;

	now = get_time_ms();
	last_compile_start = get_last_compile_start(coder);
	return (now - last_compile_start
		>= coder->simulation->config.time_to_burnout);
}

void	update_dongle_cooldown(t_dongle *dongles, t_simulation *simulation)
{
	int	i;

	pthread_mutex_lock(&simulation->state_mutex);
	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (dongles[i].state == DONGLE_COOLDOWN
			&& dongles[i].cooldown_until <= get_time_ms())
		{
			dongles[i].state = DONGLE_FREE;
			pthread_cond_broadcast(&simulation->state_cond);
		}
		i++;
	}
	pthread_mutex_unlock(&simulation->state_mutex);
}

int	all_coders_finished(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (get_compile_count(&simulation->coders[i])
			< simulation->config.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	set_simulation_finished(t_simulation *simulation, int value)
{
	pthread_mutex_lock(&simulation->state_mutex);
	simulation->finished = value;
	pthread_cond_broadcast(&simulation->state_cond);
	pthread_mutex_unlock(&simulation->state_mutex);
}
