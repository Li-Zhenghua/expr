#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/file.h>
#include <semaphore.h>

#define N 10
#define PN 3
#define CN 2

int buf[N];

pthread_t p_tid[PN];
pthread_t c_tid[CN];

sem_t empty_sem;
sem_t full_sem;
int in, out, i;

pthread_mutex_t mutex;
FILE* file;


void *init_p(void *arg)
{
    int temp = 0;
    while (1)
    {
        sem_wait(&empty_sem);
        pthread_mutex_lock(&mutex);
        //critical region
	if ((fscanf(file, "%d", &temp) < 0 ))
        {
	    pthread_mutex_unlock(&mutex);
            sem_post(&full_sem);
            break;
        }
        buf[in] = temp;
        printf("producer %d produce %d \n", (int)arg, buf[in]);

        in = (in + 1) % N;

        fflush(NULL);
        pthread_mutex_unlock(&mutex);
        sem_post(&full_sem);
        sleep(1);
    }
}
void *init_c(void *arg)
{
    int temp;
    while (1)
    {
        sem_wait(&full_sem);
        pthread_mutex_lock(&mutex);
        // critical region
        temp = buf[out];
        printf("comsumer %d comsume %d \n", (int)arg, temp);
        out = (out + 1) % N;
        fflush(NULL);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
        sleep(1);
    }
}

int main()
{
    sem_init(&empty_sem, 0, N);
    sem_init(&full_sem, 0, 0);
    srand(time(0));
    pthread_mutex_init(&mutex, NULL);

    file = fopen("data.txt", "r");

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
