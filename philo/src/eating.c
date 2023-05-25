/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:37:58 by etattevi          #+#    #+#             */
/*   Updated: 2023/05/25 17:40:18 by etattevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

bool	is_philo_done_eating(t_seat *seat)
{
	static bool	done_eating = false;

	if (pthread_mutex_lock(&seat->meals_mutex))
		return (seat->errored = true, false);
	if (seat->meals_eaten == seat->philos->meals_nb)
	{
		seat->meals_eaten++;
		done_eating = true;
	}
	if (pthread_mutex_unlock(&seat->meals_mutex))
		return (seat->errored = true, false);
	return (done_eating);
}

bool	is_done_eating(t_seat *seat)
{
	bool	done_eating;

	if (pthread_mutex_lock(&seat->meals_mutex))
		return (seat->errored = true, true);
	done_eating = seat->meals_eaten >= seat->philos->meals_nb
		&& seat->philos->meals_nb != -1;
	if (pthread_mutex_unlock(&seat->meals_mutex))
		return (seat->errored = true, true);
	return (done_eating);
}
