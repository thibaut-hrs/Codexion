/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 12:01:55 by thours            #+#    #+#             */
/*   Updated: 2026/08/21 14:59:52 by thours           ###   ########.fr       */
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
        if (all_coders_finished(simulation))
			return (set_simulation_finished(simulation, 1), NULL);
		usleep(1000);
	}
	return (NULL);
}

long long	get_last_compile_start(t_coder *coder)
{
    long long   last_compile_start;

    pthread_mutex_lock(&coder->simulation->state_mutex);
    last_compile_start = coder->last_compile_start;
    pthread_mutex_unlock(&coder->simulation->state_mutex);
    return (last_compile_start);
}

int	get_compile_count(t_coder *coder)
{
	int	compile_count;

	pthread_mutex_lock(&coder->simulation->state_mutex);
	compile_count = coder->compile_count;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
	return (compile_count);
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