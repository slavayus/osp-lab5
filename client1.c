#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main() {
    int shm_id = shmget(KEY, sizeof(server_data *), 0);

    if (shm_id < 0) {
        perror("client:shmget");
        exit(EXIT_FAILURE);
    }

    server_data *p_server_data = (server_data *) shmat(shm_id, NULL, SHM_RDONLY);
    if (p_server_data == NULL) {
        perror("client:shmat");
        exit(EXIT_FAILURE);
    }

    printf("pid = %i\nuid = %i\ngid = %i\ntime = %i\n", p_server_data->pid, p_server_data->uid, p_server_data->gid,
           (int) p_server_data->load_time);
    printf("load1 = %f; load5 = %f; load15 = %f\n", p_server_data->load_avg[0], p_server_data->load_avg[1],
           p_server_data->load_avg[2]);

    return EXIT_SUCCESS;
}

