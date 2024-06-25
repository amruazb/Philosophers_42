/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:51:41 by aabashee          #+#    #+#             */
/*   Updated: 2024/06/25 14:52:17 by aabashee         ###   ########.fr       */
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
/**
 * Prints information about a philosopher.
 * 
 * @param philo The philosopher to print information about.
 * @param print_order The type of information to print (1: took a fork, 2: eating, 3: sleeping, 4: thinking, 5: died).
 */

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
/**
 * Initializes a philosopher's information.
 * 
 * @param philo The philosopher to initialize.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 */
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
/**
 * Initializes an array of philosophers.
 * 
 * @param philo The array of philosophers to initialize.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * 
 * Example: init_philo(philo, 5, argv) initializes an array of philosophers with arguments from argv
 */
/* bool init_philo(t_philo *philo, int argc, char **argv)

The function takes three parameters:
philo: a pointer to an array of t_philo structures
argc: the number of command-line arguments (not used in this function)
argv: an array of command-line arguments (used to get the number of philosophers)
Initialization
The function allocates memory for a t_info structure using malloc. If the allocation
fails, it returns false.
It initializes the philo_info structure:
philo_dead is set to false, indicating that no philosopher has died yet.
Two mutexes are initialized: print_lock and dlock. These mutexes are likely used 
to synchronize access to shared resources.
Loop to initialize philosophers

The function then loops ft_atoi(argv[1]) times, where ft_atoi is a custom function 
to convert a string to an integer. This loop initializes each philosopher in the array:

philo[i].philo_info is set to the shared philo_info structure.
philo[i].id is set to i + 1, assigning a unique ID to each philosopher.
init_info is called to initialize additional information for each philosopher (not shown in this code snippet).
The current timestamp is obtained using gettimeofday and stored in philo[i].life_t.
The life field is set to the timestamp value using the n_timestamp function (not shown in this code snippet).
The next field is set to point to the next philosopher in the array, creating a circular linked list. 
If this is the last philosopher, next points back to the first philosopher.
Return

The function returns true if all philosophers are successfully initialized.

In summary, this function sets up the shared information structure and initializes an array of philosopher structures, 
each with its own ID, timestamp, and links to neighboring philosophers. */
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
/**
 * Checks for errors in the command-line arguments.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * 
 * Example: ft_error(5, argv) returns true if the arguments are valid, false otherwise
 */
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
