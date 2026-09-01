/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thours <thours@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 18:22:06 by thours            #+#    #+#             */
/*   Updated: 2026/09/01 11:40:19 by thours           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	parse_args(int argc, char **argv, t_config *config)
{
	if (argc != 9)
		return (0);
	if (!parse_positive_int(argv[1], &config->number_of_coders))
		return (0);
	if (!parse_positive_ll(argv[2], &config->time_to_burnout))
		return (0);
	if (!parse_positive_ll(argv[3], &config->time_to_compile))
		return (0);
	if (!parse_positive_ll(argv[4], &config->time_to_debug))
		return (0);
	if (!parse_positive_ll(argv[5], &config->time_to_refactor))
		return (0);
	if (!parse_positive_int(argv[6], &config->number_of_compiles_required))
		return (0);
	if (!parse_positive_ll(argv[7], &config->dongle_cooldown))
		return (0);
	if (!parse_scheduler(argv[8], &config->scheduler))
		return (0);
	return (1);
}

int	parse_positive_ll(char *str, long long *value)
{
	int			i;
	long long	result;
	int			digit;

	i = 0;
	result = 0;
	if (!str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		digit = str[i] - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*value = result;
	return (1);
}

int	parse_positive_int(char *str, int *value)
{
	int		i;
	int		result;
	int		digit;

	i = 0;
	result = 0;
	if (!str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		digit = str[i] - '0';
		if (result > (INT_MAX - digit) / 10)
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*value = result;
	return (1);
}

int	parse_scheduler(char *str, t_scheduler *scheduler)
{
	if (strcmp(str, "fifo") == 0)
	{
		*scheduler = SCHEDULER_FIFO;
		return (1);
	}
	if (strcmp(str, "edf") == 0)
	{
		*scheduler = SCHEDULER_EDF;
		return (1);
	}
	return (0);
}
