/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:52:14 by aabashee          #+#    #+#             */
/*   Updated: 2024/04/01 15:39:10 by aabashee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static bool	check_mirror_fork(t_philo *philo)
{
	if (philo->m_fork == philo->id || philo->next->m_fork == philo->id)
		return (true);
	return (false);
}

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
