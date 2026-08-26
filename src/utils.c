/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 09:57:19 by thours            #+#    #+#             */
/*   Updated: 2026/08/26 19:13:07 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;
	long long		milliseconds;

	gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (milliseconds);
}

int	get_simulation_finished(t_simulation *simulation)
{
	int	finished;

	pthread_mutex_lock(&simulation->state_mutex);
	finished = simulation->finished;
	pthread_mutex_unlock(&simulation->state_mutex);
	return (finished);
}

void	set_simulation_finished(t_simulation *simulation, int value)
{
	pthread_mutex_lock(&simulation->state_mutex);
	simulation->finished = value;
	pthread_mutex_unlock(&simulation->state_mutex);
}
