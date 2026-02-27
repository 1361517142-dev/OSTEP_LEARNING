#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int fill = 0;
int use = 0;
int count = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void put(int val) {
    buffer[fill] = val;
    fill = (fill + 1) % BUFFER_SIZE;
    count++;
    return;
}

int get(int val) {
    int ret = buffer[use];
    use = (use + 1) % BUFFER_SIZE;
    count--;
    return ret;
}

void* producer(void* arg) {
    (void)arg;

    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&lock);
        
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&empty, &lock);
        }

        put(i);
        printf("product: %d\n", i);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
    }
}

void* consumer(void* arg) {
    (void)arg;

    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&lock);
        
        while (count == 0) {
            pthread_cond_wait(&full, &lock);
        }

        int gt = get(i);
        printf("consume: %d\n", gt);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&lock);
    }
}

