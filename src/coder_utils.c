/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:26:47 by thours            #+#    #+#             */
/*   Updated: 2026/09/24 17:37:01 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	set_last_compile_start(t_coder *coder, long long time)
{
	pthread_mutex_lock(&coder->simulation->state_mutex);
	coder->last_compile_start = time;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->simulation->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
}

void	log_event(t_coder *coder, const char *message)
{
	pthread_mutex_t	log_mutex;

	log_mutex = coder->simulation->log_mutex;
	pthread_mutex_lock(&log_mutex);
	printf("%lld %d %s\n",
		get_time_ms() - coder->simulation->start_time, coder->id, message);
	pthread_mutex_unlock(&log_mutex);
}

void	log_event_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_t	log_mutex;

	log_mutex = coder->simulation->log_mutex;
	pthread_mutex_lock(&log_mutex);
	printf("%lld %d has taken dongle %d\n",
		get_time_ms() - coder->simulation->start_time, coder->id, dongle->id);
	pthread_mutex_unlock(&log_mutex);
}

int	has_free_dongles(t_coder *coder)
{
	if (!coder->left_dongle || !coder->right_dongle)
		return (0);
	return (coder->left_dongle->state == DONGLE_FREE
		&& coder->right_dongle->state == DONGLE_FREE);
}
