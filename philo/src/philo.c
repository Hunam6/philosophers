/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:24:21 by hunam             #+#    #+#             */
/*   Updated: 2023/05/25 17:38:27 by etattevi         ###   ########.fr       */
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

bool	has_anyone_died(t_seat *seat)
{
	bool	death;

	if (pthread_mutex_lock(&seat->philos->died_mutex))
		return (seat->errored = true, true);
	death = seat->philos->has_anyone_died;
	if (pthread_mutex_unlock(&seat->philos->died_mutex))
		return (seat->errored = true, true);
	return (death);
}

void	set_has_anyone_died(t_seat *seat)
{
	if (pthread_mutex_lock(&seat->philos->died_mutex))
	{
		seat->errored = true;
		return ;
	}
	seat->philos->has_anyone_died = true;
	if (pthread_mutex_unlock(&seat->philos->died_mutex))
		seat->errored = true;
}
