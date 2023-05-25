/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dying.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:48:35 by etattevi          #+#    #+#             */
/*   Updated: 2023/05/25 17:49:21 by etattevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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

bool	is_philo_dead(t_seat *seat)
{
	const long	ts = time_since_ts(&seat->last_meal);

	return (ts > seat->philos->time_to_die || ts == -1);
}

bool	kill_philosophy(t_philos *philos)
{
	int	i;

	i = -1;
	while (++i < philos->philos_nb)
		if (pthread_join(philos->seats[i].thread, NULL))
			return (false);
	i = -1;
	while (++i < philos->philos_nb)
	{
		if (pthread_mutex_destroy(&philos->seats[i].fork))
			return (false);
		if (pthread_mutex_destroy(&philos->seats[i].meals_mutex))
			return (false);
	}
	if (pthread_mutex_destroy(&philos->died_mutex))
		return (false);
	return (true);
}
