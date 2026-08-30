/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 19:00:04 by thours            #+#    #+#             */
/*   Updated: 2026/08/30 13:21:54 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	simulation;

	if (!parse_args(argc, argv, &config))
	{
		printf("ERROR: Invalid input\n");
		return (1);
	}
	if (!init_simulation(&simulation, config))
	{
		printf("ERROR: failed to create the simulation\n");
		return (1);
	}
	if (!start_simulation(&simulation))
	{
		printf("ERROR: failed to start the simulation\n");
		destroy_simulation(&simulation);
		return (1);
	}
	join_threads(&simulation);
	if (all_coders_finished(&simulation))
		printf("Simulation succesfully ended without burn out !\n");
	destroy_simulation(&simulation);
	return (0);
}