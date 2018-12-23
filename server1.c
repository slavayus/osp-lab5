#include <stdlib.h>
#include "common.h"
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

void init_server_data(server_data *p_server_data);

int main() {
    time_t start_time = time(NULL);

    int shm_id = shmget(KEY, sizeof(server_data *), IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);

    if (shm_id < 0) {
        perror("server:shmget");
        exit(EXIT_FAILURE);
    }

    server_data *p_server_data = (server_data *) shmat(shm_id, NULL, 0);
    if (p_server_data == NULL) {
        perror("server:shmat");
        exit(EXIT_FAILURE);
    }

    init_server_data(p_server_data);

    while(1) {
        p_server_data->load_time = (time_t)difftime(time(NULL), start_time);
        getloadavg(p_server_data->load_avg, 3);
        sleep(1);
    }

    return EXIT_SUCCESS;
}

void init_server_data(server_data *p_server_data) {
    p_server_data->pid = getpid();
    p_server_data->uid = getuid();
    p_server_data->gid = getgid();
    p_server_data->load_time = 0;
    getloadavg(p_server_data->load_avg, 3);
}