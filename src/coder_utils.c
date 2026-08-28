/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 22:26:47 by thours            #+#    #+#             */
/*   Updated: 2026/08/28 22:32:08 by thours           ###   ########.fr       */
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

int	coder_can_compile(t_coder *coder);