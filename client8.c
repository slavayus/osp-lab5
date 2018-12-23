#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include "common.h"

int main() {
    struct sockaddr_un sock_addr;
    int socket_id;

    if ((socket_id = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("client:socket");
        exit(EXIT_FAILURE);
    }

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, filename);

    if (connect(socket_id, (const struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
        perror("client:connect");
        exit(EXIT_FAILURE);
    }

    server_data p_server_data;
    if (read(socket_id, &p_server_data, sizeof(p_server_data)) != sizeof(p_server_data)) {
        perror("client:read");
        exit(EXIT_FAILURE);
    }

    printf("pid = %i\nuid = %i\ngid = %i\ntime = %i\n", p_server_data.pid, p_server_data.uid, p_server_data.gid,
           (int) p_server_data.load_time);
    printf("load1 = %f; load5 = %f; load15 = %f\n", p_server_data.load_avg[0], p_server_data.load_avg[1],
           p_server_data.load_avg[2]);
    close(socket_id);

    return EXIT_SUCCESS;
}

