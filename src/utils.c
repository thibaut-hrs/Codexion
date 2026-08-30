/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 09:57:19 by thours            #+#    #+#             */
/*   Updated: 2026/08/29 11:21:55 by thours           ###   ########.fr       */
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

long long	get_last_compile_start(t_coder *coder)
{
	long long	last_compile_start;

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
