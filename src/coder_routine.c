/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 10:17:34 by thours            #+#    #+#             */
/*   Updated: 2026/09/01 11:32:34 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!get_simulation_finished(coder->simulation))
	{
		coder_debug(coder);
		if (get_simulation_finished(coder->simulation))
			break ;
		coder_refactor(coder);
		if (get_simulation_finished(coder->simulation))
			break ;
		coder_compile(coder, &coder->simulation->queue);
	}
	return (NULL);
}

void	coder_debug(t_coder *coder)
{
	coder->state = STATE_DEBUGGING;
	log_event(coder, "is debugging");
	usleep(coder->simulation->config.time_to_debug * 1000);
}

void	coder_refactor(t_coder *coder)
{
	coder->state = STATE_REFACTORING;
	log_event(coder, "is refactoring");
	usleep(coder->simulation->config.time_to_refactor * 1000);
}

void	coder_compile(t_coder *coder, t_priority_queue *queue)
{
	long long	cooldown;

	cooldown = coder->simulation->config.dongle_cooldown;
	if (!create_compile_request(coder, queue))
		return ;
	if (!try_start_compile(coder, queue))
		return ;
	coder->state = STATE_COMPILING;
	set_last_compile_start(coder, get_time_ms());
	log_event(coder, "is compiling");
	usleep(coder->simulation->config.time_to_compile * 1000);
	pthread_mutex_lock(&coder->simulation->state_mutex);
	coder->left_dongle->state = DONGLE_COOLDOWN;
	coder->left_dongle->cooldown_until = get_time_ms() + cooldown;
	coder->right_dongle->state = DONGLE_COOLDOWN;
	coder->right_dongle->cooldown_until = get_time_ms() + cooldown;
	pthread_mutex_unlock(&coder->simulation->state_mutex);
	increment_compile_count(coder);
}
