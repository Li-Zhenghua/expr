#include <pthread.h>
#include <stdio.h>
#define N 5

#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINK_TIME 3
#define EAT_TIME 2

enum
{
    THINKING,
    HUNGRY,
    EATING
} state[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER, s[N];

int i;
void try_getfork(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        pthread_mutex_unlock(&s[i]);
    }
}
void take_forks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    try_getfork(i);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&s[i]);
}
void put_forks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    try_getfork(LEFT);
    try_getfork(RIGHT);
    pthread_mutex_unlock(&mutex);
}
void think(int i)
{
    printf("thread %d is thinking\n", i);
    sleep(THINK_TIME);
}
void eat(int i)
{
    printf("thread %d is eating\n", i);
    sleep(EAT_TIME);
}
void *init_phi(void *arg)
{
    int i = (int)arg;
    for (;;)
    {
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
    return NULL;
}
int main()
{
    pthread_t tid[N];
    for (i = 0; i < N; i++)
        pthread_create(&tid[i], NULL, init_phi, (void *)i);
    for (i = 0; i < N; i++)
        pthread_join(tid[i], NULL);
    return 0;
}
