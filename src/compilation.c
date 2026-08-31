/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compilation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 11:13:25 by thours            #+#    #+#             */
/*   Updated: 2026/08/31 13:31:12 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static int is_simulation_finished(t_simulation *simulation)
{
	return (simulation->finished);
}

int	coder_can_compile(t_coder *coder, t_priority_queue *queue)
{
	if (is_simulation_finished(coder->simulation))
		return (0);
	if (queue->size == 0 || queue->requests[0].coder != coder)
		return (0);
	if (!coder->left_dongle || !coder->right_dongle)
		return (0);
	if (coder->left_dongle->state != DONGLE_FREE)
		return (0);
	if (coder->right_dongle->state != DONGLE_FREE)
		return (0);
	return (1);
}

int	try_start_compile(t_coder *coder, t_priority_queue *queue)
{
	t_simulation	*simulation;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->state_mutex);
	while (!coder_can_compile(coder, queue))
	{
		if (is_simulation_finished(simulation))
		{
			pthread_mutex_unlock(&simulation->state_mutex);
			return (0);
		}
		pthread_cond_wait(&simulation->state_cond,
			&simulation->state_mutex);
	}
	coder->left_dongle->state = DONGLE_HELD;
	log_event_dongle(coder, coder->left_dongle);
	coder->right_dongle->state = DONGLE_HELD;
	log_event_dongle(coder, coder->right_dongle);
	queue_pop(queue);
	pthread_cond_broadcast(&simulation->state_cond);
	pthread_mutex_unlock(&simulation->state_mutex);
	return (1);
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