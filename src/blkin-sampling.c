#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <blkin-sampling.h>

struct sampling_info *create_shm()
{
    key_t key;
    int shmid;
    struct sampling_info *data;
    pid_t pid;
    
    /*get pid to use as key for the shared mem*/
    pid = getpid();

    /*  create the segment: */
    if ((shmid = shmget(pid, sizeof(struct sampling_info), 
                    0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    data = shmat(shmid, (void *)0, 0);
    if (data == (struct sampling_info *)(-1)) {
        perror("shmat");
        exit(1);
    }

    return data;
}

void send_pid()
{
    int s, t, len;
    struct sockaddr_un remote;

    /*Connect*/
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected.\n");
    pid_t pid;
    
    /*get pid to use as key for the shared mem*/
    pid = getpid();
    printf("I am data and my pid is %d\n", pid);
    send(s, &pid, sizeof(pid_t), 0); 
    close(s);      
}

struct sampling_info *sampling_init()
{
    struct sampling_info *info = create_shm();
    info->rate = 1;
    info->req_count = 0;
    send_pid();
    return info;
}

#if 0
int main()
{
    struct sampling_info *info = create_shm();
    info->rate = 1;
    info->req_count = 0;
    send_pid();
    int count = 0;
    /*Simulate*/
    while (1) {
        info->req_count++;
        count++;
        if (count % 10000 == 0) {
            printf("Data: rate = %d\n", info->rate);
            count = 0;
        }
        usleep(500);
    }
}
#endif
