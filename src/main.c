/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 19:00:04 by thours            #+#    #+#             */
/*   Updated: 2026/08/21 11:57:15 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int main(int argc, char **argv)
{
	t_config    *config;

	config = malloc(sizeof(t_config));
	if (!config)
		return (1);
	if (!parse_args(argc, argv, config))
		return (free(config), 1);
	t_simulation *simulation = malloc(sizeof(t_simulation));
	if (init_simulation(simulation, *config))
	{
		if (!start_simulation(simulation))
		{
			printf("Error creating threads\n");
			destroy_simulation(simulation);
			free(config);
			free(simulation);
			return (1);
		}
		if (!join_threads(simulation))
		{
			printf("Error joining threads\n");
			destroy_simulation(simulation);
			free(config);
			free(simulation);
			return (1);
		}
	}
	destroy_simulation(simulation);
	free(config);
	free(simulation);
	
	return (0);
}