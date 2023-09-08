/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anepali <anepali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:01:20 by anepali           #+#    #+#             */
/*   Updated: 2023/07/28 14:02:21 by anepali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_in_strings(char c, char *str)
{
	while (*str)
	{
		if (*str == c)
			return (true);
		str++;
	}
	return (false);
}

long	ft_atoi(const char *str)
{
	int		sign;
	long	result;

	while (is_in_strings(*(char *) str, "\t\n\v\f\r "))
		str++;
	sign = 1;
	if (*str == '-')
		sign *= -1;
	if (*str == '-' || *str == '+')
		str++;
	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (result * 10 < 0)
			return (-1);
		result *= 10;
		result += *str - '0';
		str++;
	}
	return (result * sign);
}

void init_philos(t_data *data)
{
    int i;

    i = 0;
    while(i < data->philo_num)
    {
        
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].time_to_die = data->death_time;
		data->philos[i].eat_cont = 0;
/**
 * The function "input_check" checks if the command line arguments are valid by ensuring that they only
 * contain numeric characters.
 * 
 * @param argv The parameter `argv` is a pointer to a pointer to a character. It is typically used to
 * pass command line arguments to a program. In this case, it is used to check the validity of the
 * input.
 * 
 * @return an integer value. If the input check is successful and there are no errors, it will return
 * 0. Otherwise, it will return an error code.
 */
		data->philos[i].eating = 0;
		data->philos[i].status = 0;
		pthread_mutex_init(&data->philos[i].lock, NULL);
		i++;
    }

}

int init_forks(t_data *data)
{
    int i;

    i = -1;
    while( ++i < data->philo_num)
        pthread_mutex_init(&data->forks[i], NULL);
    i = 0;
    data->philos[0].l_fork = &data->forks[0];
    data->philos[0].r_fork = &data->forks[data->philo_num - 1];
    i = 1;
    while(i < data->philo_num)
    {
        data->philos[i].l_fork = &data->forks[i];
        data->philos[i].r_fork = &data->forks[i - 1];
        i++;
    }
    return (0);
}

/**
 * The function `int_mem` allocates memory for thread IDs, forks, and philosophers in a dining
 * philosophers problem.
 * 
 * @param data A pointer to a struct of type t_data.
 * 
 * @return an integer value. If the memory allocation is successful, it returns 0. If there is an error
 * in memory allocation, it returns an error code.
 */
int	int_mem(t_data *data)
{
    data->thread_id = malloc(sizeof(pthread_t) * data->philo_num);
    if (!data->thread_id)
        return (error(ALLOC_ERR_1, data));
    data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
    if (!data->forks)
        return (error(ALLOC_ERR_2, data));
    data->philos = malloc(sizeof(t_philo) * data->philo_num);
    if (!data->philos)
        return (error(ALLOC_ERR_3, data));
    return (0);
}

/**
 * The function initializes the data structure with values from command line arguments and initializes
 * mutexes.
 */
int init_data(t_data *data, char **argv, int argc)
{
    data->philo_num = ft_atoi(argv[1]);
    data->death_time = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        data->meals_nb = ft_atoi(argv[5]);
    else
        data->meals_nb = -1;
    data->dead = 0;
    data->finished = 0;
    pthread_mutex_init(&data->write, NULL);
    pthread_mutex_init(&data->lock, NULL);
    return (0);
}

int	init(t_data *data, char **argv, int argc)
{
   /* The code `if (init_data(data, argv, argc)) return (1);` is calling the `init_data` function and
   checking its return value. If the return value is non-zero (indicating an error), the function
   returns 1, indicating that there was an error in initializing the data structure. */
    if (init_data(data, argv, argc))
        return (1);
    if (int_mem(data))
        return (1);
    if (init_forks(data))
        return (1);
    init_philos(data);
    return (0);
}
