#include <stdio.h>
#include <pthread.h>

#define N 5
#define PN 3
#define CN 4


int cache_pool[N];

pthread_t p_tid[PN];
pthread_t c_tid[CN];

sem_t empty_sem;
sem_t full_sem;
int in, out;

pthread_mutex_t mutex;

void *init_p(void *arg)
{
    FILE *file = fopen("data.txt", 'r');
    int temp;
    while (1)
    {
        // read data from file
        int lock = flock(fileno(file), LOCK_SH);
        if ((offset = fscanf(file, "%d", i)) != 0)
        {
            temp = i;
            fseek(file, offset, SEEK_CUR);
        }
        int release = flock(fileno(file), LOCK_UN);
        sem_wait(empty_sem);
        pthread_mutex_lock(&mutex);
        //critical region
        cache_pool[in] = temp;
        in = (in + 1) % N;
        

        pthread_mutex_unlock(&mutex);
        sem_post(empty_sem);
    }
}
void *init_c(void *arg)
{
    while (1)
    {
        sem_wait(full_sem);
        pthread_mutex_lock(&mutex);
        // critical region
        temp = cache_pool[out];
        out = (out + 1) % n;
        printf("comsume %d", temp);

        pthread_mutex_unlock(&mutex);
        sem_post(full_sem);
    }
}

int main()
{
    sem_init(&empty_sem, 0, N);
    sem_init(&full_sem, 0, 0);


    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < PN; i++)
        pthread_create(&p_tid[i], NULL, init_p, (void *)i);
    for (i = 0; i < CN; i++)
        pthread_create(&c_tid[i], NULL, init_c, (void *)i);


    for (i = 0; i < PN; i++)
        pthread_join(p_tid[i], NULL);
    for (i = 0; i < CN; i++)
        pthread_join(c_tid[i], NULL);

    return 0;
}