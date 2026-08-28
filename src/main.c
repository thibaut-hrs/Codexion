/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 19:00:04 by thours            #+#    #+#             */
/*   Updated: 2026/08/28 21:57:41 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static void	print_request(t_request *request)
{
	printf("Coder %d | order: %d | deadline: %lld\n",
		request->coder->id,
		request->order,
		request->deadline);
}

static t_request	create_request(t_coder *coder, int order, long long deadline)
{
	t_request	request;

	request.coder = coder;
	request.order = order;
	request.deadline = deadline;
	return (request);
}

static void	print_heap(t_priority_queue *queue)
{
	int	i;

	i = 0;
	while (i < queue->size)
	{
		printf("[%d] coder %d | order %d | deadline %lld\n",
			i,
			queue->requests[i].coder->id,
			queue->requests[i].order,
			queue->requests[i].deadline);
		i++;
	}
}

static void	test_fifo(void)
{
	t_priority_queue	queue;
	t_coder			coders[6];
	t_request		request;
	int				i;

	printf("\n===== FIFO =====\n");

	i = 0;
	while (i < 5)
	{
		coders[i].id = i + 1;
		i++;
	}

	queue_init(&queue, SCHEDULER_FIFO);

	queue_push(&queue, create_request(&coders[2], 3, 300));
	queue_push(&queue, create_request(&coders[0], 1, 500));
	queue_push(&queue, create_request(&coders[4], 5, 100));
	queue_push(&queue, create_request(&coders[1], 2, 200));
	queue_push(&queue, create_request(&coders[3], 4, 400));

	while (queue_pop(&queue, &request))
	{
		print_request(&request);
	}

	queue_destroy(&queue);
}

static void	test_edf(void)
{
	t_priority_queue	queue;
	t_coder			coders[6];
	t_request		request;
	int				i;

	printf("\n===== EDF =====\n");

	i = 0;
	while (i < 5)
	{
		coders[i].id = i + 1;
		i++;
	}

	queue_init(&queue, SCHEDULER_EDF);

	queue_push(&queue, create_request(&coders[2], 3, 300));
	queue_push(&queue, create_request(&coders[5], 6, 300));
	queue_push(&queue, create_request(&coders[0], 1, 500));
	queue_push(&queue, create_request(&coders[4], 5, 100));
	queue_push(&queue, create_request(&coders[1], 2, 200));
	queue_push(&queue, create_request(&coders[3], 4, 400));

	print_heap(&queue);
	while (queue_pop(&queue, &request))
		print_request(&request);

	queue_destroy(&queue);
}

int	main(void)
{
	test_fifo();
	test_edf();
	return (0);
}