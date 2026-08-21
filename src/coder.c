/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 10:17:34 by thours            #+#    #+#             */
/*   Updated: 2026/08/21 10:39:34 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("Coder %d started\n", coder->id);
	return (NULL);
}

int	start_simulation(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.number_of_coders)
	{
		if (pthread_create(
				&simulation->coders[i].thread,
				NULL,
				coder_routine,
				&simulation->coders[i]) != 0)
			return (0);
		simulation->threads_created++;
		i++;
	}
	return (1);
}

int	join_threads(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->threads_created)
	{
		if (pthread_join(simulation->coders[i].thread, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}