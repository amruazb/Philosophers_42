/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 05:36:18 by aabashee          #+#    #+#             */
/*   Updated: 2024/04/01 15:52:06 by aabashee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	*handle_single_philo(t_philo *philo, int ttime)
{
	size_t	ts;
	size_t	target;
	long	n_time;

	(void)ttime;
	ts = timestamp_new();
	target = ts + (ttime * 1000);
	while (ts < target)
	{
		n_time = n_timestamp(&philo->life_t);
		if ((n_time - philo->life) > (philo->philo_info->time_to_die))
			break ;
		ts = timestamp_new();
	}
	print_info(philo, 5);
	return (NULL);
}

static bool	philo_sleep(t_philo *philo)
{
	size_t	ts;
	size_t	target;

	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(&philo->next->lock);
	ts = timestamp_new();
	target = ts + (philo->philo_info->time_to_sleep * 1000);
	while (ts < target)
	{
		if (is_philo_dead(philo))
			return (false);
		usleep(100);
		ts = timestamp_new();
	}
	if (philo->id % philo->philo_info->num_of_philo)
	{
		pthread_mutex_lock(&philo->lock);
		pthread_mutex_lock(&philo->next->lock);
	}
	else
	{
		pthread_mutex_lock(&philo->next->lock);
		pthread_mutex_lock(&philo->lock);
	}
	return (true);
}

static bool	t_sleep(t_philo *philo, int eating_time)
{
	size_t	ts;
	size_t	target;

	ts = timestamp_new();
	target = ts + (eating_time * 1000);
	while (ts < target)
	{
		if (is_philo_dead(philo))
			return (false);
		usleep(100);
		ts = timestamp_new();
	}
	return (true);
}

static bool	finish_times_to_eat(t_philo *philo)
{
	if (philo->times_to_eat == 0)
	{
		pthread_mutex_unlock(&philo->lock);
		pthread_mutex_unlock(&philo->next->lock);
		return (true);
	}
	return (false);
}

bool	philo_eat(t_philo *philo)
{
	philo->fork = 1;
	philo->next->fork = 1;
	philo->m_fork = philo->id;
	philo->next->m_fork = philo->id;           // leave your id to the fork
	philo->life = n_timestamp(&philo->life_t); // update the life time
	pthread_mutex_unlock(&philo->rlock);       //
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(&philo->next->lock);
	print_info(philo, 1);
	print_info(philo, 1);
	print_info(philo, 2);
	philo->times_to_eat--;
	if (!t_sleep(philo, philo->philo_info->time_to_eat))
		return (false);
	lock_order(philo);
	philo->fork = 0;
	philo->next->fork = 0;
	if (finish_times_to_eat(philo))
		return (false);
	print_info(philo, 3);
	if (!philo_sleep(philo))
		return (false);
	print_info(philo, 4);
	pthread_mutex_lock(&philo->rlock);
	return (true);
}
