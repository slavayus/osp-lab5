#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include "common.h"

server_data init_server_data() {
    server_data sd;
    sd.pid = getpid();
    sd.uid = getuid();
    sd.gid = getgid();
    sd.load_time = 0;
    getloadavg(sd.load_avg, 3);
    return sd;
}

int main() {
    time_t start_time = time(NULL);

    unlink(filename);

    int socket_id;
    if ((socket_id = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("server:socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, filename);

    if (bind(socket_id, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
        perror("server:bind");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_id, 5) == -1) {
        perror("server:listen");
        exit(EXIT_FAILURE);
    }

    server_data sd_p = init_server_data();
    int cl;

    while (1) {
        sd_p.load_time = (time_t) difftime(time(NULL), start_time);
        getloadavg(sd_p.load_avg, 3);
        if ((cl = accept(socket_id, NULL, NULL)) != -1) {
            write(cl, &sd_p, sizeof(sd_p));
        }
        sleep(1);
    }
}

