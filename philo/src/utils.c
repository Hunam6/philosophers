/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:39:13 by hunam             #+#    #+#             */
/*   Updated: 2023/05/25 17:50:05 by etattevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(const char *str)
{
	long	nbr;
	int		i;
	int		is_neg;

	i = 0;
	nbr = 0;
	is_neg = 0;
	while (str[i] != '\0' && (str[i] == 32 || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f'))
		i++;
	if (str[i] != '\0' && str[i] == '-')
	{
		is_neg = 1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while ('0' <= str[i] && str[i] <= '9')
		nbr = nbr * 10 + str[i++] - '0';
	if (is_neg == 1)
		return (-nbr);
	return (nbr);
}

long	time_since_ts(struct timeval *a)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL))
		return (-1);
	return (((current_time.tv_sec - a->tv_sec) * 1000000
			+ (current_time.tv_usec - a->tv_usec)) / 1000);
}

t_seat	*right_seat(t_seat *seat)
{
	if (seat->id == seat->philos->philos_nb - 1)
		return (&seat->philos->seats[0]);
	else
		return (&seat->philos->seats[seat->id + 1]);
}

bool	log_msg(t_seat *seat, t_log_msg msg)
{
	const static char		*msgs[] = {"has taken a fork", "is eating",
		"is sleeping", "is thinking", "died"};
	static pthread_mutex_t	print_mutex = PTHREAD_MUTEX_INITIALIZER;
	const long				ts = time_since_ts(&seat->philos->start_time);

	if (ts == -1)
		return (false);
	if (pthread_mutex_lock(&print_mutex))
		return (false);
	if (has_anyone_died(seat))
	{
		if (pthread_mutex_unlock(&print_mutex))
			return (false);
		return (true);
	}
	printf("%ld %d %s\n", ts, seat->id + 1, msgs[msg]);
	if (msg == die)
		set_has_anyone_died(seat);
	if (pthread_mutex_unlock(&print_mutex))
		return (false);
	return (true);
}
