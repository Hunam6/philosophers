/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:24:21 by hunam             #+#    #+#             */
/*   Updated: 2023/05/25 17:48:44 by etattevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	philo_routine(t_seat *seat)
{
	if (seat->id % 2)
		if (usleep(seat->philos->time_to_eat))
			seat->errored = true;
	while (42)
	{
		if (only_one_philo(seat)
			|| is_done_eating(seat) || has_anyone_died(seat))
			return ;
		if (!eat(seat))
			seat->errored = true;
		if (is_done_eating(seat) || has_anyone_died(seat))
			return ;
		if (!log_msg(seat, sleeping))
			seat->errored = true;
		if (usleep(seat->philos->time_to_sleep * 1000))
			seat->errored = true;
		if (!log_msg(seat, thinking))
			seat->errored = true;
	}
}

bool	only_one_philo(t_seat *seat)
{
	if (seat->philos->philos_nb != 1)
		return (false);
	if (pthread_mutex_lock(&seat->fork))
		return (seat->errored = true, true);
	if (!log_msg(seat, take_a_fork))
		return (seat->errored = true, true);
	if (usleep(seat->philos->time_to_die * 1000))
		return (seat->errored = true, true);
	if (pthread_mutex_unlock(&seat->fork))
		return (seat->errored = true, true);
	return (true);
}
