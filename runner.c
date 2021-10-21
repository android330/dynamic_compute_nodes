#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define TOTAL_TESTS 10000000

sem_t sem;

typedef struct _args{
    float* res;
    int name;
}args;

void* rpi(void* a) {
    usleep(rand() % 100);
    int succ = 0;
    sem_wait(&sem);
    printf("pi #%d has entered sun\n", ((args*)a)->name);
    for (int i = 0; i < TOTAL_TESTS; i++)
    {
        float x = 1.0 - (float)rand()/((float)RAND_MAX/2.0);
        float y = 1.0 - (float)rand()/((float)RAND_MAX/2.0);
        // printf("x %f, y %f\n", x, y);
        float dist = sqrt(x*x + y*y);
        if (dist <= 1) succ++;
    }
    float out = ((float)succ / (float)TOTAL_TESTS) * 4.0;
    printf("pi #%d thinks res is %f\n", ((args*)a)->name, out);
    sem_post(&sem);
    *(((args *)a)->res) = out; // bruh
    return NULL;
}

int main(void) {
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_sec * t1.tv_usec);
    sem_init(&sem, 0, 2);
    pthread_t cluster[5];
    float res[5];
    for (int i = 0; i < 5; i++) {
        // printf("starting pi #%d\n", i);
        args *a = malloc(sizeof(args));
        a->res = res + i;
        a->name = i;
        pthread_create(cluster+i, (const pthread_attr_t*)NULL, rpi, (void*)a);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(cluster[i], NULL);
        printf("pi %d returned %f\n", i, res[i]);
    }
    
    return 0;
}