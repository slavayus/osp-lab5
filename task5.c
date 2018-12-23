#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include "common.h"

#define ALPHSIZE 26

char alphabet[ALPHSIZE] = "abcdefghijklmnopqrstuvwxyz";

void *change_order();

void *change_register();

pthread_t t[2];
int semid;

int main() {
    struct sembuf buf = {0, 1, SEM_UNDO};
    struct sembuf sem_print = {2, -1, SEM_UNDO};
    semid = semget(KEY, 3, IPC_CREAT | 0600);

    pthread_create(&t[0], NULL, change_register, NULL);
    pthread_create(&t[1], NULL, change_order, NULL);

    while (1) {
        buf.sem_num = (unsigned short) ((buf.sem_num + 1) % 2);
        semop(semid, &buf, 1);
        semop(semid, &sem_print, 1);
        printf("%s\n", alphabet);
        sleep(1);
    }
}

void *change_order() {
    char tmp;
    struct sembuf lock = {0, -1, SEM_UNDO};
    struct sembuf unlock = {2, 1, SEM_UNDO};
    while (semop(semid, &lock, 1) != -1) {
        for (int i = 0; i < ALPHSIZE / 2; i++) {
            tmp = alphabet[i];
            alphabet[i] = alphabet[ALPHSIZE - i - 1];
            alphabet[ALPHSIZE - i - 1] = tmp;
        }
        semop(semid, &unlock, 1);
    }
    return NULL;
}

void *change_register() {
    struct sembuf lock = {1, -1, SEM_UNDO};
    struct sembuf unlock = {2, 1, SEM_UNDO};
    while (semop(semid, &lock, 1) != -1) {
        for (int i = 0; i < ALPHSIZE; i++) {
            alphabet[i] += (alphabet[i] - 'A') < ALPHSIZE ? 32 : -32;
        }
        semop(semid, &unlock, 1);
    }
    return NULL;
}
