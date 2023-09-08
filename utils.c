/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anepali <anepali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:02:11 by anepali           #+#    #+#             */
/*   Updated: 2023/07/28 12:53:29 by anepali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 != '\0' && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(char *)s1 - *(char *)s2);
}


int	ft_usleep(useconds_t time)
{
	u_int64_t	start;
	start = get_time();
	while ((get_time() - start) < time)
		usleep(time / 10);
	return(0);
}

void pick_forks(t_philo *philo)
{
    pthread_mutex_lock(philo->r_fork);
    pthread_mutex_lock(philo->l_fork);
}

u_int64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (error("gettimeofday() FAILURE\n", NULL));
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

void	messages(char *str, t_philo *philo)
{
	u_int64_t	time;

	pthread_mutex_lock(&philo->data->write);
	time = get_time() - philo->data->start_time;
    if (ft_strcmp(DIED, str) == 0 && philo->data->dead == 0)
	{
		printf("%lu %d %s\n", time, philo->id, str);
		philo->data->dead = 1;
	}
	if (!philo->data->dead)
		printf("%lu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);
}

void    drop_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->r_fork);
    pthread_mutex_unlock(philo->l_fork);
    messages(SLEEPING, philo);
    ft_usleep(philo->data->time_to_sleep);
}

/**
 * The function "eat" is responsible for a philosopher picking up forks, eating for a specified amount
 * of time, and then dropping the forks.
 * 
 * @param philo The parameter "philo" is of type "t_philo", which is a struct that represents a
 * philosopher. It likely contains information about the philosopher's state, such as whether they are
 * currently eating, the time at which they need to die, and the number of times they have eaten.
 */
void eat(t_philo *philo)
{
    pick_forks(philo);
    pthread_mutex_lock(&philo->lock);
    philo->eating = 1;
    philo->time_to_die = get_time() + philo->data->death_time;
    messages(EATING, philo);
    philo->eat_cont++;
    ft_usleep(philo->data->time_to_eat);
    philo->eating = 0;
    pthread_mutex_unlock(&philo->lock);
    drop_forks(philo);
}
