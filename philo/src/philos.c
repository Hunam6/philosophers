/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:24:21 by hunam             #+#    #+#             */
/*   Updated: 2023/05/25 17:37:55 by etattevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	main(int ac, char **av)
{
	t_philos	philos;

	if (ac < 5 || ac > 6)
		return (write(2, "You should input 4-5 parameters.\n", 34));
	if (!init_philos(&philos, ac, av))
		return (write(2, "Initialization failed.\n", 24));
	if (!check_philos(&philos))
		return (write(2, "Routine failed.\n", 16));
	if (!kill_philosophy(&philos))
		return (write(2, "Philosophy won.\n", 17));
}

bool	init_philos(t_philos *philos, int ac, char **av)
{
	memset(philos, 0, sizeof(t_philos));
	philos->philos_nb = ft_atoi(av[1]);
	philos->time_to_die = ft_atoi(av[2]);
	philos->time_to_eat = ft_atoi(av[3]);
	philos->time_to_sleep = ft_atoi(av[4]);
	philos->meals_nb = -1;
	if (!is_input_valid(philos, ac, av))
		return (false);
	if (ac == 6)
		philos->meals_nb = ft_atoi(av[5]);
	if (pthread_mutex_init(&philos->died_mutex, NULL))
		return (false);
	if (gettimeofday(&philos->start_time, NULL))
		return (false);
	if (!init_each_philo(philos))
		return (false);
	return (true);
}

bool	init_each_philo(t_philos *philos)
{
	int	i;

	i = -1;
	while (++i < philos->philos_nb)
	{
		if (pthread_mutex_init(&philos->seats[i].fork, NULL))
			return (false);
		if (pthread_mutex_init(&philos->seats[i].meals_mutex, NULL))
			return (false);
		philos->seats[i].id = i;
		philos->seats[i].errored = false;
		philos->seats[i].last_meal = philos->start_time;
		philos->seats[i].philos = philos;
		if (pthread_create(&philos->seats[i].thread, NULL,
				(void *(*)(void *))philo_routine, &philos->seats[i]))
			return (false);
	}
	return (true);
}

bool	is_input_valid(t_philos *philos, int ac, char **av)
{
	if (philos->philos_nb < 1 || philos->philos_nb > 200
		|| philos->time_to_die < 60
		|| philos->time_to_eat < 60
		|| philos->time_to_sleep < 60
		|| (ac == 6 && ft_atoi(av[5]) < 0))
		return (false);
	return (true);
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


bool	check_philos(t_philos *philos)
{
	static int	philos_done_eating = 0;
	int			i;

	while (42)
	{
		i = -1;
		while (++i < philos->philos_nb)
		{
			if (philos->seats[i].errored)
				return (false);
			if (is_philo_dead(&philos->seats[i]))
				return (log_msg(&philos->seats[i], die));
			if (philos->seats[i].errored)
				return (false);
			philos_done_eating += is_philo_done_eating(&philos->seats[i]);
			if (philos_done_eating == philos->philos_nb)
				return (true);
			if (usleep(5000 / philos->philos_nb))
				return (false);
		}
	}
}
