#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

// Define a estrutura do filósofo
typedef struct s_philos {
  int index;
  pthread_t philo;
  pthread_mutex_t fork;
  long int last_time_eat;
  struct s_philos *next;
  struct s_philos *previous;
} t_philos;

// Função para pegar os garfos (modificada para usar estrutura)
void pick_up_forks(t_philos *philosopher) {
  pthread_mutex_lock(&philosopher->fork);
  pthread_mutex_lock(&(philosopher->next->fork));
}

// Função para soltar os garfos (modificada para usar estrutura)
void put_down_forks(t_philos *philosopher) {
  pthread_mutex_unlock(&(philosopher->next->fork));
  pthread_mutex_unlock(&philosopher->fork);
}

// Função do filósofo (modificada para usar estrutura)
void *philosopher(void *arg) {
  t_philos *philosopher = (t_philos *)arg;

  while (1) {
    // Pensando
    printf("Filósofo %d pensando\n", philosopher->index);
    usleep(10000);

    // Pega os garfos
    pick_up_forks(philosopher);

    // Comendo
    printf("Filósofo %d comendo\n", philosopher->index);
    usleep(20000);

    // Solta os garfos
    put_down_forks(philosopher);
  }

  return NULL;
}

int main() {
  int i;
  t_philos philosophers[NUM_PHILOSOPHERS];

  // Inicializa os filósofos e mutexes
  for (i = 0; i < NUM_PHILOSOPHERS; i++) {
    philosophers[i].index = i;
    pthread_mutex_init(&philosophers[i].fork, NULL);
  }

  // Conecta os filósofos em uma circular
  for (i = 0; i < NUM_PHILOSOPHERS; i++) {
    philosophers[i].next = &philosophers[(i + 1) % NUM_PHILOSOPHERS];
    philosophers[i].previous = &philosophers[(i - 1 + NUM_PHILOSOPHERS) % NUM_PHILOSOPHERS];
  }

  // Cria as threads
  for (i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_create(&philosophers[i].philo, NULL, philosopher, &philosophers[i]);
  }

  // Espera todas as threads terminarem
  for (i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_join(philosophers[i].philo, NULL);
  }

  return 0;
}
