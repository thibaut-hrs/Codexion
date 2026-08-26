/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 18:06:36 by thours            #+#    #+#             */
/*   Updated: 2026/08/26 19:34:41 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	request_has_priority(t_priority_queue *queue, t_request *a, t_request *b)
{
	if (queue->scheduler == SCHEDULER_FIFO)
		return (a->order < b->order);
	if (a->deadline < b->deadline)
		return (1);
	if (a->deadline == b->deadline && a->order < b->order)
		return (1);
	return (0);
}
