/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 19:00:04 by thours            #+#    #+#             */
/*   Updated: 2026/08/21 10:02:25 by thours           ###   ########.fr       */
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
        printf("Error during parsing\n");
    else
        printf("Parsing succes\n");
    t_simulation *simulation = malloc(sizeof(t_simulation));
    init_simulation(simulation, *config);
    free(config);
    free(simulation);
    
    return (0);
}