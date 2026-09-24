/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compilation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 11:13:25 by thours            #+#    #+#             */
/*   Updated: 2026/09/24 19:25:14 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static int	is_simulation_finished(t_simulation *simulation)
{
	return (simulation->finished);
}

int	try_start_compile(t_coder *coder, t_priority_queue *queue)
{
	int				index;

	pthread_mutex_lock(&coder->simulation->state_mutex);
	while (1)
	{
		if (is_simulation_finished(coder->simulation))
		{
			pthread_mutex_unlock(&coder->simulation->state_mutex);
			return (0);
		}
		index = find_best_available_request(queue);
		if (index >= 0 && queue->requests[index].coder == coder)
		{
			coder->left_dongle->state = DONGLE_HELD;
			log_event_dongle(coder, coder->left_dongle);
			coder->right_dongle->state = DONGLE_HELD;
			log_event_dongle(coder, coder->right_dongle);
			queue_remove(queue, index);
			pthread_cond_broadcast(&coder->simulation->state_cond);
			pthread_mutex_unlock(&coder->simulation->state_mutex);
			return (1);
		}
		pthread_cond_wait(&coder->simulation->state_cond,
			&coder->simulation->state_mutex);
	}
}

int	create_compile_request(t_coder *coder, t_priority_queue *queue)
{
	t_simulation	*simulation;
	t_request		request;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->state_mutex);
	request.coder = coder;
	request.order = simulation->next_request_order++;
	request.deadline = coder->last_compile_start
		+ simulation->config.time_to_burnout;
	if (!queue_push(queue, request))
	{
		pthread_mutex_unlock(&simulation->state_mutex);
		return (0);
	}
	pthread_cond_broadcast(&simulation->state_cond);
	pthread_mutex_unlock(&simulation->state_mutex);
	return (1);
}
