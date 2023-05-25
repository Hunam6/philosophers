/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:37:58 by etattevi          #+#    #+#             */
/*   Updated: 2023/05/25 18:01:52 by etattevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

bool	eat(t_seat *seat)
{
	if (!is_done_eating(seat))
	{
		if (!grab_forks(seat))
			return (false);
		if (!log_msg(seat, eating))
			return (false);
		if (gettimeofday(&seat->last_meal, NULL))
			return (false);
		if (pthread_mutex_lock(&seat->meals_mutex))
			return (false);
		seat->meals_eaten++;
		if (pthread_mutex_unlock(&seat->meals_mutex))
			return (false);
		if (usleep(seat->philos->time_to_eat * 1000))
			return (false);
		if (pthread_mutex_unlock(&right_seat(seat)->fork))
			return (false);
		if (pthread_mutex_unlock(&seat->fork))
			return (false);
	}
	return (true);
}

bool	is_done_eating(t_seat *seat)
{
	bool	done_eating;

	if (pthread_mutex_lock(&seat->meals_mutex))
		return (seat->errored = true, false);
	done_eating = (seat->meals_eaten >= seat->philos->meals_nb
			&& seat->philos->meals_nb != -1);
	if (done_eating)
		seat->meals_eaten++;
	if (pthread_mutex_unlock(&seat->meals_mutex))
		return (seat->errored = true, false);
	return (done_eating);
}

bool	grab_forks(t_seat *seat)
{
	if (pthread_mutex_lock(&seat->fork))
		return (false);
	if (!log_msg(seat, take_a_fork))
		return (false);
	if (pthread_mutex_lock(&right_seat(seat)->fork))
		return (false);
	if (!log_msg(seat, take_a_fork))
		return (false);
	return (true);
}
