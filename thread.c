/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anepali <anepali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:01:11 by anepali           #+#    #+#             */
/*   Updated: 2023/07/12 15:23:06 by anepali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * The function "monitor" checks if all philosophers have finished eating and updates the "dead"
 * variable accordingly.
 * 
 * @param data_pointer A void pointer that points to the data structure of a philosopher (t_philo).
 * 
 * @return a NULL pointer.
 */
void	*monitor(void *data_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) data_pointer;
	pthread_mutex_lock(&philo->data->write);
	printf("data val: %d", philo->data->dead);
	pthread_mutex_unlock(&philo->data->write);
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->finished >= philo->data->philo_num)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

/**
 * The supervisor function checks if a philosopher has died or if they have finished eating their
 * designated number of meals.
 * 
 * @param philo_pointer A void pointer that points to a t_philo struct, which represents a philosopher
 * in a dining philosophers problem.
 * 
 * @return a NULL pointer.
 */
void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->time_to_die && philo->eating == 0)
			messages(DIED, philo);
		if (philo->eat_cont == philo->data->meals_nb)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			philo->eat_cont++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

/**
 * The function "routine" creates a thread for a philosopher, sets the time for their death, and
 * continuously calls the "eat" function until the philosopher is dead.
 * 
 * @param philo_ptr A void pointer to the philosopher struct.
 * 
 * @return a void pointer.
 */
void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	philo->time_to_die = philo->data->death_time + get_time();
	if (pthread_create(&philo->t1, NULL, &supervisor, (void *)philo))
		return ((void *)1);
	while (philo->data->dead == 0)
	{
		eat(philo);
		messages(THINKING, philo);
	}
	if (pthread_join(philo->t1, NULL))
		return ((void *)1);
	return ((void *)0);
}

/**
 * The function "thread_start" creates and joins threads for monitoring and executing the routine for
 * each philosopher.
 * 
 * @param data A pointer to a structure of type t_data, which contains information about the thread and
 * its execution.
 * 
 * @return an integer value. The value returned is 0 if the function executes successfully.
 */

int thread_start(t_data *data)
{
    int			i;
	pthread_t	t0;

	i = -1;
	data->start_time = get_time();
	if (data->meals_nb > 0)
	{
		if (pthread_create(&t0, NULL, &monitor, &data->philos[0]))
			return (error(TH_ERR, data));
	}
    while(++i < data->philo_num)
    {
        if (pthread_create(&data->thread_id[i], NULL, &routine, &data->philos[i]))
            return (error(TH_ERR, data));
		ft_usleep(1);
    }
    i = -1;
	while (++i < data->philo_num)
	{
		if (pthread_join(data->thread_id[i], NULL))
			return (error(JOIN_ERR, data));
	}
	return (0);
}