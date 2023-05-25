/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etattevi <etattevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:18:38 by hunam             #+#    #+#             */
/*   Updated: 2023/05/25 17:42:34 by etattevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <string.h>

typedef enum e_log_type {
	take_a_fork,
	eating,
	sleeping,
	thinking,
	die,
}	t_log_msg;

typedef struct s_philos	t_philos;

typedef struct s_seat {
	int				id;
	pthread_t		thread;
	pthread_mutex_t	fork;
	bool			errored;

	struct timeval	last_meal;
	int				meals_eaten;
	pthread_mutex_t	meals_mutex;

	t_philos		*philos;
}	t_seat;

typedef struct s_philos {
	int				philos_nb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_nb;

	struct timeval	start_time;
	bool			has_anyone_died;
	pthread_mutex_t	died_mutex;

	t_seat			seats[200];
}	t_philos;

//philos.c
bool	init_philos(t_philos *philos, int ac, char **av);
bool	init_each_philo(t_philos *philos);
bool	is_input_valid(t_philos *philos, int ac, char **av);
bool	kill_philosophy(t_philos *philos);
bool	check_philos(t_philos *philos);

//utils.c
int		ft_atoi(const char *str);
bool	is_philo_dead(t_seat *seat);
bool	log_msg(t_seat *seat, t_log_msg msg);
t_seat	*right_seat(t_seat *seat);
long	time_since_ts(struct timeval *a);

//eating.c
bool	is_philo_done_eating(t_seat *seat);

//philo.c
bool	get_forks(t_seat *seat);
bool	eat(t_seat *seat);
bool	only_one_philo(t_seat *seat);
bool	is_done_eating(t_seat *seat);
bool	has_anyone_died(t_seat *seat);
void	set_has_anyone_died(t_seat *seat);
void	philo_routine(t_seat *seat);
bool	grab_forks(t_seat *seat);

//TODO. make sure every function is prototyped

#endif