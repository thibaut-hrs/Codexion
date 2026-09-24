/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:55:49 by thours            #+#    #+#             */
/*   Updated: 2026/09/24 19:27:14 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	queue_init(t_priority_queue *queue, t_scheduler scheduler)
{
	queue->size = 0;
	queue->capacity = INITIAL_QUEUE_CAPACITY;
	queue->scheduler = scheduler;
	queue->requests = malloc(sizeof(t_request) * queue->capacity);
	if (!queue->requests)
		return (0);
	return (1);
}

int	queue_grow(t_priority_queue *queue)
{
	t_request	*new_requests;
	int			new_size;

	new_size = queue->capacity * 2;
	new_requests = realloc(queue->requests, sizeof(t_request) * new_size);
	if (!new_requests)
		return (0);
	queue->requests = new_requests;
	queue->capacity = new_size;
	return (1);
}

int	queue_push(t_priority_queue *queue, t_request request)
{
	if (queue->size == queue->capacity)
	{
		if (!queue_grow(queue))
			return (0);
	}
	queue->requests[queue->size] = request;
	queue->size++;
	heap_up(queue, queue->size - 1);
	return (1);
}

int	queue_remove(t_priority_queue *queue, int index)
{
	if (index < 0 || index >= queue->size)
		return (0);
	if (index == queue->size - 1)
	{
		queue->size--;
		return (1);
	}
	queue->requests[index] = queue->requests[queue->size - 1];
	queue->size--;
	if (index > 0 && request_has_priority(
			queue, &queue->requests[index], &queue->requests[(index - 1) / 2]))
		heap_up(queue, index);
	else
		heap_down(queue, index);
	return (1);
}

void	queue_destroy(t_priority_queue *queue)
{
	free(queue->requests);
	queue->requests = NULL;
	queue->size = 0;
	queue->capacity = 0;
}
