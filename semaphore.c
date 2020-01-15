#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct semaphore{
    pthread_cond_t *cond;
    pthread_mutex_t *mtx;
    volatile unsigned N;
}sem;


int counter = 0;

void sem_init(sem * ps, int N){
    pthread_mutex_init(&ps->mtx, NULL);
    pthread_cond_init(&ps->cond, NULL);
    ps->N = N;
}

void sem_destroy(sem * ps){
    pthread_mutex_destroy(&ps->mtx);
    pthread_cond_destroy(&ps->cond);
}

void sem_acquire(sem * ps){
    pthread_mutex_lock(&ps->mtx);
    while(ps->N == 0){
        pthread_cond_wait(&ps->cond,&ps->mtx);
    }
    --(ps->N);
    pthread_mutex_unlock(&ps->mtx);
}

void sem_release( sem * ps){
    pthread_mutex_lock(&ps->mtx);
    ++(ps->N);
    pthread_mutex_unlock(&ps->mtx);
    pthread_cond_signal(&ps->cond);
}


void *thread_func(void * semaphore){
    sem *mysem = semaphore;
    sem_acquire(mysem);
    int Z = 100;
    for(int i =0; i< Z; i++){
        counter++;
    }
    sem_release(mysem);
    return NULL;
}


int main(void){
    sem *Mysem = malloc(sizeof(*Mysem));

    Mysem->mtx = malloc(sizeof(*(Mysem->mtx)));
    Mysem->cond = malloc(sizeof(*(Mysem->cond)));
    Mysem->N = malloc(sizeof(Mysem->N));
    free(Mysem);
    // SEM_INIT(1, Mysem);
    pthread_cond_init(&Mysem->cond ,NULL);
    pthread_mutex_init(&Mysem->mtx, NULL);
    Mysem->N = 1;
    int N = 5;
    pthread_t thread[N];
    int i =0;
    for(i = 0; i< N; i++){
        pthread_create(&thread[i], NULL,thread_func, &Mysem);
    }
    int z = 0;
    for(z =0; z<N; z++){
        pthread_join(thread[z], NULL);
    }

    printf("%d\n", counter);
    return EXIT_SUCCESS;
}