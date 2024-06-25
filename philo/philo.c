/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:52:14 by aabashee          #+#    #+#             */
/*   Updated: 2024/06/25 16:15:57 by aabashee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Checks if a philosopher is dead.
 *
 * This function checks if a philosopher has died by comparing the current time
 * with the last time the philosopher ate. If the time difference exceeds the
 * philosopher's time to die, the function sets the philosopher's death flag to
 * true and returns 1. Otherwise, it returns 0.
 *
 * @param philo A pointer to the philosopher structure.
 * @return 1 if the philosopher is dead, 0 otherwise.
 *
 * Example:
 * ```c
 * t_philo philo;
 * // Initialize philo structure...
 * if (is_philo_dead(&philo)) {
 *     printf("Philosopher %d has died!\n", philo.id);
 * }
 * ```
 */
int	is_philo_dead(t_philo *philo)
{
	long	n_time;
	int		flag;

	flag = 0;
	ft_mutex(philo, 1);
	if (philo->philo_info->philo_dead)
		flag = 1;
	else
	{
		n_time = n_timestamp(&philo->life_t);
		if ((n_time - philo->life) > (philo->philo_info->time_to_die))
		{
			ft_mutex(philo, 0);
			print_info(philo, 5);
			ft_mutex(philo, 1);
			philo->philo_info->philo_dead = true;
			flag = 1;
		}
	}
	ft_mutex(philo, 0);
	return (flag);
}
/**
 * Checks if the philosopher has a mirror fork or the next philosopher has a mirror fork.
 *
 * A mirror fork is a fork that is held by the philosopher with the same id as the current philosopher.
 *
 * @param philo The philosopher to check.
 * @return true if the philosopher has a mirror fork or the next philosopher has a mirror fork, false otherwise.
 *
 * Example:
 * If philo->id is 3 and philo->m_fork is 3, or philo->next->m_fork is 3, the function returns true.
 * If philo->id is 3 and philo->m_fork is 2 and philo->next->m_fork is 1, the function returns false.
 */
static bool	check_mirror_fork(t_philo *philo)
{
	if (philo->m_fork == philo->id || philo->next->m_fork == philo->id)
		return (true);
	return (false);
}
/**
 * The routine function is responsible for managing the life cycle of a philosopher.
 * It takes a void pointer to a t_philo struct as an argument, which contains the philosopher's data.
 * The function runs in an infinite loop, where the philosopher eats, sleeps, and thinks.
 * If the philosopher is alone (num_of_philo == 1), it will call the handle_single_philo function.
 * Otherwise, it will follow the standard eating and sleeping pattern.
 * The function returns NULL if the philosopher dies or finishes eating the specified number of times.
 *
 * Example:
 * ```c
 * t_philo philo;
 * // Initialize philo data
 * pthread_create(&thread, NULL, routine, &philo);
 * ```
 *
 * @param philo_data A void pointer to a t_philo struct containing the philosopher's data.
 * @return NULL if the philosopher dies or finishes eating the specified number of times.
 */
static void	*routine(void *philo_data)
{
	t_philo	*philo;

	philo = (t_philo *)philo_data;
	while (philo->philo_info->num_of_philo == 1)
		return (handle_single_philo(philo, philo->philo_info->time_to_die));
	while (true)
	{
		if (is_philo_dead(philo) || !philo->times_to_eat)
			break ;
		lock_order(philo);
		pthread_mutex_lock(&philo->rlock);
		if (!(philo->fork) && !(philo->next->fork)
			&& philo->times_to_eat && !check_mirror_fork(philo))//check the id already took the fork
			if (!philo_eat(philo))
				return (NULL);
		pthread_mutex_unlock(&philo->rlock);
		pthread_mutex_unlock(&philo->lock);
		pthread_mutex_unlock(&philo->next->lock);
		usleep(100);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	philo[MAX_PHILOSOPHERS];
	int		i;

	i = 0;
	if (!ft_error(argc, argv))
		return (1);
	if (!init_philo(philo, argc, argv))
		return (1);
	while (i < ft_atoi(argv[1]))
	{
		pthread_create(&philo[i].thread, NULL, routine, (void *)&philo[i]);
		usleep(100);
		i++;
	}
	i = -1;
	while (++i < ft_atoi(argv[1]))
		pthread_join(philo[i].thread, NULL);
	free(philo->philo_info);
	return (0);
}
