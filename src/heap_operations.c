/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 19:33:13 by thours            #+#    #+#             */
/*   Updated: 2026/08/28 14:36:24 by thours           ###   ########.fr       */
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

void	heap_swap(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	heap_up(t_priority_queue *queue, int index)
{
	int	parent_index;

	while (index != 0)
	{
		parent_index = (index - 1) / 2;
		if (!request_has_priority(queue,
				&queue->requests[index],
				&queue->requests[parent_index]))
			break;
		heap_swap(&queue->requests[index],
			&queue->requests[parent_index]);
		index = parent_index;
	}
}

void	heap_down(t_priority_queue *queue, int index)
{
	int	child_index;

	while (2 * index + 1 < queue->size)
	{
		child_index = 2 * index + 1;
		if (2 * index + 2 < queue->size &&
			request_has_priority(queue, &queue->requests[2 * index + 2],
				&queue->requests[2 * index + 1]))
			child_index = 2 * index + 2;
		if (request_has_priority(queue,
				&queue->requests[index],
				&queue->requests[child_index]))
			break;
		heap_swap(&queue->requests[index],
			&queue->requests[child_index]);
		index = child_index;
	}
}
