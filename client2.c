#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include "common.h"

int main() {
    int ms_id = msgget(KEY, IPC_CREAT | 0666);
    if (ms_id < 0) {
        perror("client:msgget");
        exit(EXIT_FAILURE);
    }

    server_data p_server_data;
    if (msgrcv(ms_id, &p_server_data, sizeof(p_server_data), 0, 0) < 0) {
        perror("client:msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("pid = %i\nuid = %i\ngid = %i\ntime = %i\n", p_server_data.pid, p_server_data.uid, (int) p_server_data.gid,
           (int) p_server_data.load_time);
    printf("load1 = %f; load5 = %f; load15 = %f\n", p_server_data.load_avg[0], p_server_data.load_avg[1],
           p_server_data.load_avg[2]);

    return EXIT_SUCCESS;
}
