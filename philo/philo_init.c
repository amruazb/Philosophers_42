/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:51:41 by aabashee          #+#    #+#             */
/*   Updated: 2024/03/26 09:56:07 by aabashee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	unsigned int	num;
	int				negative;

	num = 0;
	negative = 1;
	while (*str == ' ' || (*str >= 19 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			negative = -negative;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		num = (num * 10) + (*str - '0');
		str++;
	}
	if (num > 2147483648 && negative == -1)
		return (0);
	if ((num > 2147483647 && negative == 1) || (*str || (str[0] == '+'
				&& !str[1]) || (str[0] == '-' && !str[1])))
		return (-1);
	str = NULL;
	return (num * negative);
}

void	print_info(t_philo *philo, int print_order)
{
	ft_mutex(philo, 1);
	if (!philo->philo_info->philo_dead)
	{
		pthread_mutex_lock(&philo->philo_info->print_lock);
		printf("%s", COLOR_CYAN);
		printf("%ld", n_timestamp(&philo->life_t));
		printf("%s %d%s", COLOR_RESET, philo->id, COLOR_RESET);
		if (print_order == 1)
			printf(" %s%s has taken a fork\n", COLOR_YELLOW, EMOJI_FORK);
		else if (print_order == 2)
			printf(" %s%s is eating\n", COLOR_GREEN, EMOJI_EATING);
		else if (print_order == 3)
			printf(" %s%s is %s sleeping\n", COLOR_GREEN,
				EMOJI_SLEEPING, COLOR_DARK_CORNFLOWER_BLUE);
		else if (print_order == 4)
			printf(" %s%s is %s thinking\n", COLOR_GREEN,
				EMOJI_THINKING, COLOR_PURPLE);
		else if (print_order == 5)
		{
			printf(" %s%s died\n", COLOR_RED, EMOJI_DEAD);
			philo->philo_info->philo_dead = true;
		}
		pthread_mutex_unlock(&philo->philo_info->print_lock);
	}
	ft_mutex(philo, 0);
}

static	void	init_info(t_philo *philo, int argc, char **argv)
{
	philo->fork = 0;
	philo->m_fork = 0;
	philo->philo_info->time_to_die = ft_atoi(argv[2]);
	philo->philo_info->time_to_eat = ft_atoi(argv[3]);
	philo->philo_info->time_to_sleep = ft_atoi(argv[4]);
	pthread_mutex_init(&philo->lock, NULL);
	pthread_mutex_init(&philo->rlock, NULL);
	philo->philo_info->num_of_philo = ft_atoi(argv[1]);
	if (argc == 6)
		philo->times_to_eat = ft_atoi(argv[5]);
	else
		philo->times_to_eat = -1;
}

bool	init_philo(t_philo *philo, int argc, char **argv)
{
	int		i;
	t_info	*philo_info;

	i = 0;
	philo_info = malloc(sizeof(t_info));
	if (!philo_info)
		return (false);
	philo_info->philo_dead = false;
	pthread_mutex_init(&philo_info->print_lock, NULL);
	pthread_mutex_init(&philo_info->dlock, NULL);
	while (i < ft_atoi(argv[1]))
	{
		philo[i].philo_info = philo_info;
		philo[i].id = i + 1;
		init_info(&philo[i], argc, argv);
		gettimeofday(&philo[i].life_t, NULL);
		philo[i].life = n_timestamp(&philo[i].life_t);
		if (i == (ft_atoi(argv[1]) - 1))
			philo[i].next = &philo[0];
		else
			philo[i].next = &philo[i + 1];
		i++;
	}
	return (true);
}

bool	ft_error(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Error : Run philo using ./philo 2 800 200 200 \
					((7) -- optional)");
		return (false);
	}
	i = 1;
	while (argv[i])
	{
		if (*argv[i] == '\0' || ft_atoi(argv[i]) <= 0)
		{
			printf("Error : Run philo using ./philo 2 800 200 200 \
						((7) --- optional)");
			return (false);
		}
		i++;
	}
	return (true);
}
