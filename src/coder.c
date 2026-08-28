/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 10:17:34 by thours            #+#    #+#             */
/*   Updated: 2026/08/28 22:29:09 by thours           ###   ########.fr       */
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
		coder_compile(coder);
	}
	return (NULL);
}

void	coder_debug(t_coder *coder)
{
	coder->state = STATE_DEBUGGING;
	printf("%lld %d is debugging\n",
		get_time_ms() - coder->simulation->start_time, coder->id);
	usleep(coder->simulation->config.time_to_debug * 1000);
}

void	coder_refactor(t_coder *coder)
{
	coder->state = STATE_REFACTORING;
	printf("%lld %d is refactoring\n",
		get_time_ms() - coder->simulation->start_time, coder->id);
	usleep(coder->simulation->config.time_to_refactor * 1000);
}

void	coder_compile(t_coder *coder)
{
	coder->state = STATE_COMPILING;
	set_last_compile_start(coder, get_time_ms());
	printf("%lld %d is compiling\n",
		get_time_ms() - coder->simulation->start_time, coder->id);
	usleep(coder->simulation->config.time_to_compile * 1000);
	increment_compile_count(coder);
}
