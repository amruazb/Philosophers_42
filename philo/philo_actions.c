/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 05:36:18 by aabashee          #+#    #+#             */
/*   Updated: 2024/06/25 15:02:52 by aabashee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* handle_single_philo function

This function is called when there is only one philosopher. It takes a t_philo struct 
and an integer ttime as arguments. The function:

Creates a new timestamp ts using the timestamp_new() function.
Calculates a target timestamp target by adding ttime milliseconds to ts.
Enters a loop that continues until ts reaches target.
Inside the loop, it checks if the philosopher's life time has exceeded 
the time to die (philo->philo_info->time_to_die). If so, it breaks out of the loop.
Prints a message indicating that the philosopher has died (code 5) */
/**
 * Handles a single philosopher's life cycle.
 *
 * This function simulates a philosopher's life cycle, checking if the philosopher
 * has died (i.e., the time elapsed since the last meal exceeds the time to die).
 * If the philosopher has died, the function breaks out of the loop and prints a
 * death message. Otherwise, the function continues to simulate the philosopher's
 * life cycle until the specified time has elapsed.
 *
 * @param philo A pointer to the philosopher's structure.
 * @param ttime The time to simulate the philosopher's life cycle (in seconds).
 * @return NULL
 *
 * Example:
 * ```c
 * t_philo philo;
 * // Initialize philo structure...
 * handle_single_philo(&philo, 10); // Simulate philosopher's life cycle for 10 seconds
 * ```
 */
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
/* philo_sleep function
This function makes a philosopher sleep for a certain amount of time. 
It takes a t_philo struct as an argument. The function:
Unlocks the philosopher's lock and the next philosopher's lock using pthread_mutex_unlock().
Creates a new timestamp ts using the timestamp_new() function.
Calculates a target timestamp target by adding the sleep time (philo->philo_info->time_to_sleep) 
to ts.
Enters a loop that continues until ts reaches target.
Inside the loop, it checks if the philosopher is dead using the is_philo_dead() function. 
If so, it returns false.
Sleeps for 100 microseconds using usleep().
Locks the philosopher's lock and the next philosopher's lock in the correct order using 
pthread_mutex_lock() */

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
/* t_sleep function

This function makes a philosopher sleep for a certain amount of time.
 It takes a t_philo struct and an integer eating_time as arguments. The function:

Creates a new timestamp ts using the timestamp_new() function.
Calculates a target timestamp target by adding eating_time milliseconds to ts.
Enters a loop that continues until ts reaches target.
Inside the loop, it checks if the philosopher is dead using the is_philo_dead() function. 
If so, it returns false.
Sleeps for 100 microseconds using usleep(). */

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
/* finish_times_to_eat function

This function checks if a philosopher has finished eating the required number of times.
 It takes a t_philo struct as an argument. The function:

Checks if the philosopher's times_to_eat counter is 0. If so, it returns true.
Otherwise, it returns false. */
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
/* philo_eat function

This function makes a philosopher eat. It takes a t_philo struct as an argument. The function:

Sets the philosopher's fork status to 1, indicating that they are holding the fork.
Sets the next philosopher's fork status to 1, indicating that they are holding the fork.
Updates the philosopher's life time using the n_timestamp() function.
Unlocks the philosopher's read lock using pthread_mutex_unlock().
Prints a message indicating that the philosopher is eating (code 1).
Sleeps for the eating time using the t_sleep() function.
Locks the philosopher's lock and the next philosopher's lock in the correct order 
using pthread_mutex_lock().
Checks if the philosopher has finished eating the required number of times using the 
finish_times_to_eat() function. If so, it returns false.
Prints a message indicating that the philosopher is sleeping (code 3).
Calls the philo_sleep() function to make the philosopher sleep.
Prints a message indicating that the philosopher is thinking (code 4).
Returns true if the philosopher has finished eating, or false otherwise. */
/**
 * philo_eat - Simulates a philosopher eating
 *
 * This function simulates a philosopher eating by taking two forks, updating the
 * philosopher's life time, printing eating information, sleeping for a certain
 * time, and then putting the forks back.
 *
 * @param philo: A pointer to the philosopher structure
 *
 * @return: true if the philosopher eats successfully, false otherwise
 *
 * Example:
 * ```c
 * t_philo philo;
 * // Initialize philo structure
 * if (!philo_eat(&philo)) {
 *     printf("Philosopher cannot eat\n");
 *     return 1;
 * }
 * ```
 */
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
