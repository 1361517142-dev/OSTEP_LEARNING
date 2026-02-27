#include <pthread.h>
#include <stdio.h>

#define LOOP_COUNT 10000000

int counter = 0;
pthread_mutex_t lock;

void* my_thread(void* arg) {
    (void)arg;

    for (int i = 0; i < LOOP_COUNT; i++) {
        pthread_mutex_lock(&lock);
        counter++;    
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t p;
    pthread_mutex_init(&lock, NULL);
    int rc = pthread_create(&p, NULL, my_thread, NULL);
    if (rc != 0) {
        fprintf(stderr, "The thread creation failed");
        pthread_mutex_destroy(&lock);
        return -1;
    }

    for (int i = 0; i < LOOP_COUNT; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }

    pthread_join(p, NULL);
    printf("%d\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;
}