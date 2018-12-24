#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include "common.h"

server_data sd_p;
time_t start_time;


server_data init_server_data();

void sig_handler(int sig_no);

int main() {

    sd_p = init_server_data();
    start_time = time(NULL);

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = sig_handler;
    sigaction(SIGHUP, &act, NULL);
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    while (1) {
        sd_p.load_time = (time_t) difftime(time(NULL), start_time);
        getloadavg(sd_p.load_avg, 3);
        sleep(1);
    }
}

void sig_handler(int sig_no) {
    int i;
    switch (sig_no) {
        case SIGHUP:
            printf("%li\n", (long) sd_p.pid);
            break;
        case SIGINT:
            printf("%li\n", (long) sd_p.uid);
            break;
        case SIGTERM:
            printf("%li\n", (long) sd_p.gid);
            break;
        case SIGUSR1:
            printf("%li\n", (long) sd_p.load_time);
            break;
        case SIGUSR2:
            for (i = 0; i < 3; i++)
                printf("%.6lf ", sd_p.load_avg[i]);
            printf("\n");
            break;
        default:
            break;
    }
}

server_data init_server_data() {
    server_data sd;
    sd.pid = getpid();
    sd.uid = getuid();
    sd.gid = getgid();
    sd.load_time = 0;
    assert(getloadavg(sd.load_avg, 3));
    return sd;
}
