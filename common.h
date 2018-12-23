#ifndef LAB5_COMMON_H
#define LAB5_COMMON_H

#include <sys/types.h>
#include <time.h>

#define KEY 100
const char *filename = "/tmp/temp_data";

typedef struct {
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t load_time;
    double load_avg[3];
} server_data;

#endif
