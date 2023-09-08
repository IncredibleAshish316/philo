#ifndef PHILO_PHILO_H
#define PHILO_PHILO_H

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

//	alloc_err
# define ALLOC_ERR_1 "ERROR WHILE ALLOCATING THREADS IDs"
# define ALLOC_ERR_3 "ERROR WHILE ALLOCATING PHILOS"
# define ALLOC_ERR_2 "ERROR WHILE ALLOCATING FORKS"
//	input_err
# define ERR_IN_1 "INVALID INPUT CHARACTER"
# define ERR_IN_2 "INVALID INPUT VALUES"
//	pthread_err
# define TH_ERR "ERROR WHILE CREATING THREADS"
# define JOIN_ERR "ERROR WHILE JOINING THREADS"
# define INIT_ERR_1 "ERROR WHILE INIT FORKS"
//	time_err
# define TIME_ERR "UNABLE TO RETRIVE UTC"
//	philo_msg
# define TAKE_FORKS "has taken a fork"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define EATING "is eating"
# define DIED "died"

struct	s_data;

typedef struct s_philo
{
    struct s_data	*data;
    pthread_t       t1;
    int             id;
    int             eat_cont;
    int             status;
    int             eating;
    u_int64_t        time_to_die;
    pthread_mutex_t	lock;
    pthread_mutex_t	*r_fork;
    pthread_mutex_t	*l_fork;
} t_philo;

typedef struct s_data
{
    pthread_t       *thread_id;
    int             philo_num;
    int             meals_nb;
    int             dead;
    int             finished;
    t_philo         *philos;
    u_int64_t       death_time;
    u_int64_t       time_to_eat;
    u_int64_t       time_to_sleep;
    u_int64_t       start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t lock;
    pthread_mutex_t write;
} t_data;

int	error(char *str, t_data *data);
void	ft_exit(t_data *data);
int	ft_usleep(useconds_t time);
u_int64_t	get_time(void);
void eat(t_philo *philo);
void	messages(char *str, t_philo *philo);
void	*routine(void *philo_ptr);
int	init(t_data *data, char **argv, int argc);
int thread_start(t_data *data);
#endif //PHILO_PHILO_H
