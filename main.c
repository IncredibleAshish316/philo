/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anepali <anepali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:02:11 by anepali           #+#    #+#             */
/*   Updated: 2023/08/08 15:24:58 by anepali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error(char *str, t_data *data)
{
	printf("%s\n", str);
	if (data)
		ft_exit(data);
	return (1);
}

void	clear_data(t_data	*data)
{
	if (data->thread_id)
		free(data->thread_id);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}

void	ft_exit(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	clear_data(data);
}

/**
 * The function `case_one` creates a thread, detaches it, and waits for a condition to be met before
 * exiting.
 * 
 * @param data A pointer to a structure of type t_data.
 * 
 * @return an integer value of 0.
 */
int	case_one(t_data *data)
{
	data->start_time = get_time();
	if (pthread_create(&data->thread_id[0], NULL, &routine, &data->philos[0]))
		return (error(TH_ERR, data));
	pthread_detach(data->thread_id[0]);
	while (data->dead == 0)
		ft_usleep(0);
	ft_exit(data);
	return (0);
}

int	input_check(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == ' ')
			{
				j++;
				continue ;
			}
			if ((argv[i][j] < 48 || argv[i][j] > 57))
				return (error(ERR_IN_1, NULL));
			j++;
		}
		i++;
	}
	return (0);
}

/**
 * The main function checks the command line arguments, initializes data, starts threads, and exits the
 * program.
 * 
 * @param argc The `argc` parameter represents the number of command-line arguments passed to the
 * program. It includes the name of the program itself as the first argument.
 * @param argv An array of strings containing the command-line arguments passed to the program. The
 * first element (argv[0]) is the name of the program itself, and the following elements are the
 * arguments provided by the user.
 * 
 * @return The main function is returning an integer value. If the conditions in the if statements are
 * met, it will return 1. If none of the conditions are met, it will return 0.
 */
 
int	main(int argc, char **argv)
{
    t_data	data;

    if (argc < 5 || argc > 6)
        return (1);
    if (input_check(argv))
        return (1);
    if (init(&data, argv, argc))
        return (1);
    if (data.philo_num == 1)
        return (case_one(&data));
    if (thread_start(&data))
        return (1);
    ft_exit(&data);
    return (0);
}
