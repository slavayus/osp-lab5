#include <stdlib.h>
#include "common.h"
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define ALPHSIZE 26

char alphabet[ALPHSIZE] = "abcdefghijklmnopqrstuvwxyz";

void *change_order();

void *change_register();

pthread_t threads[2];
sem_t sem[3];

int main() {
    for (int i = 0; i < 3; i++) {
        if (sem_init(&sem[i], 0, 0) == -1) {
            perror("thread:sem_init");
            return -1;
        }
    }
    pthread_create(&threads[0], NULL, change_register, NULL);
    pthread_create(&threads[1], NULL, change_order, NULL);

    int i = 0;
    while (1) {
        i = (i + 1) % 2;
        sem_post(&sem[i]);
        sem_wait(&sem[2]);
        printf("%s\n", alphabet);
        sleep(1);
    }
}

void *change_order() {
    int i;
    char tmp;
    while (1) {
        sem_wait(&sem[1]);
        for (i = 0; i < ALPHSIZE / 2; i++) {
            tmp = alphabet[i];
            alphabet[i] = alphabet[ALPHSIZE - i - 1];
            alphabet[ALPHSIZE - i - 1] = tmp;
        }
        sem_post(&sem[2]);
    }
    return NULL;
}

void *change_register() {
    int i;
    while (1) {
        sem_wait(&sem[0]);
        for (i = 0; i < ALPHSIZE; i++) {
            alphabet[i] += (alphabet[i] - 'A') < ALPHSIZE ? 32 : -32;
        }
        sem_post(&sem[2]);
    }
    return NULL;
}
