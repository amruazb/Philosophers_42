/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabashee <aabashee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 06:52:25 by aabashee          #+#    #+#             */
/*   Updated: 2024/03/24 16:16:55 by aabashee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILOSOPHERS 200
/* ---- ANSI escape codes for colors ---- */
# define COLOR_RESET "\033[0m"
# define COLOR_RED "\033[31m"
# define COLOR_GREEN "\033[32m"
# define COLOR_YELLOW "\033[33m"
# define COLOR_CYAN "\033[36m"
# define COLOR_DARK_CORNFLOWER_BLUE "\033[34m"
# define COLOR_PURPLE "\033[35m"

/* ---- Unicode emojis ---- */
# define EMOJI_FORK "\U0001F374"
# define EMOJI_EATING "\U0001F37D"
# define EMOJI_SLEEPING "\U0001F634" 
# define EMOJI_THINKING "\U0001F914"
# define EMOJI_DEAD "\U0001F480"

typedef struct info
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	bool			philo_dead;
	int				num_of_philo;
	pthread_mutex_t	dlock;
	pthread_mutex_t	print_lock;
}	t_info;

typedef struct philo
{
	int				id;
	int				fork;
	int				m_fork;
	long			life;
	int				times_to_eat;
	pthread_t		thread;
	pthread_mutex_t	lock;
	pthread_mutex_t	rlock;
	struct timeval	start_t;
	struct timeval	life_t;
	struct info		*philo_info;
	struct philo	*next;
}	t_philo;

/* ----- PHILO ACTION ----- */
int		*handle_single_philo(t_philo *philo, int ttime);
bool	philo_eat(t_philo *philo);
int		is_philo_dead(t_philo *philo);

/* ----- MUTEX LOCK/UNLOCK AND TIME CALCULATION FUNCTIONS ----- */
long	n_timestamp(struct timeval *time);
size_t	timestamp_new(void);
void	ft_mutex(t_philo *philo, int lock_flag);
void	lock_order(t_philo *philo);

/* ----- INITIALIZE PHILO DATA / PRINT DATA / PARSING ----- */
void	print_info(t_philo *philo, int print_order);
bool	init_philo(t_philo *philo, int argc, char **argv);
bool	ft_error(int argc, char **argv);

/* ----- UTILS FUNCTIONS ----- */
int		ft_atoi(const char	*str);

#endif