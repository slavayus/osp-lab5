#include <stdlib.h>
#include "common.h"
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>

void init_server_data(server_data p_server_data);

int main() {
    time_t start_time = time(NULL);

    int shm_id = msgget(KEY, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("server:msgget");
        exit(EXIT_FAILURE);
    }

    server_data p_server_data;
    init_server_data(p_server_data);

    while (1) {
        p_server_data.load_time = (time_t) difftime(time(NULL), start_time);
        getloadavg(p_server_data.load_avg, 3);
        if(msgsnd(shm_id, &p_server_data, sizeof(p_server_data), 0) < 0) {
            perror("client:msgsnd");
        }
        sleep(1);
    }

    return EXIT_SUCCESS;
}

void init_server_data(server_data p_server_data) {
    p_server_data.pid = getpid();
    p_server_data.uid = getuid();
    p_server_data.gid = getgid();
    p_server_data.load_time = 0;
    getloadavg(p_server_data.load_avg, 3);
}