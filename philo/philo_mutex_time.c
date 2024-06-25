/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_mutex_time.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:51:55 by aabashee          #+#    #+#             */
/*   Updated: 2024/06/25 14:53:25 by aabashee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/**
 * Calculate the time difference in milliseconds between the current time and a given time.
 *
 * @param time The time to calculate the difference from.
 * @return The time difference in milliseconds.
 *
 * Example:
 * ```c
 * struct timeval start_time;
 * gettimeofday(&start_time, NULL);
 * // Some code that takes time...
 * long time_diff = n_timestamp(&start_time);
 * printf("Time difference: %ld ms\n", time_diff);
 * ```
 */
long	n_timestamp(struct timeval *time)
{
	struct timeval	c_time;
	long			res;

	gettimeofday(&c_time, NULL);
	res = ((c_time.tv_sec - time->tv_sec) * 1000) + ((c_time.tv_usec
				- time->tv_usec) / 1000);
	return (res);
}
/**
 * Get the current timestamp in microseconds.
 *
 * @return The current timestamp in microseconds.
 *
 * Example:
 * ```c
 * size_t current_time = timestamp_new();
 * printf("Current time: %zu us\n", current_time);
 * ```
 */
size_t	timestamp_new(void)
{
	struct timeval	c_time;
	size_t			time;

	gettimeofday(&c_time, NULL);
	time = (c_time.tv_sec * 1000000 + c_time.tv_usec);
	return (time);
}
/**
 * Lock or unlock a mutex based on the lock flag.
 *
 * @param philo The philosopher structure containing the mutex.
 * @param lock_flag 1 to lock, 0 to unlock.
 *
 * Example:
 * ```c
 * t_philo philo;
 * ft_mutex(&philo, 1); // Lock the mutex
 * // Critical section...
 * ft_mutex(&philo, 0); // Unlock the mutex
 * ```
 */
void	ft_mutex(t_philo *philo, int lock_flag)
{
	if (lock_flag)
		pthread_mutex_lock(&philo->philo_info->dlock);
	else
		pthread_mutex_unlock(&philo->philo_info->dlock);
}
/**
 * Lock the mutexes in a specific order to avoid deadlocks.
 *
 * @param philo The philosopher structure containing the mutexes.
 *
 * Example:
 * ```c
 * t_philo philo;
 * lock_order(&philo); // Lock the mutexes in the correct order
 * // Critical section...
 * // Unlock the mutexes in the reverse order...
 * ```
 */
void	lock_order(t_philo *philo)
{
	if (philo->id % philo->philo_info->num_of_philo)//odd
	{
		pthread_mutex_lock(&philo->lock);
		pthread_mutex_lock(&philo->next->lock);
	}
	else//even
	{
		pthread_mutex_lock(&philo->next->lock);
		pthread_mutex_lock(&philo->lock);
	}
}
