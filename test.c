/*
 * In this example we have 2 processes communicating over a pipe.
 * We are going to trace the communication with our library
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "zipkin_c.h"

struct message {
    char actual_message[20];
    struct blkin_trace_info trace_info;
};

void process_a(int fd) 
{
    printf("I am process A: %d\n", getpid());
    struct message m = {.actual_message = "test"};
    write(fd, &m, sizeof(struct message));
}

void process_b(int fd) 
{
    printf("I am process B: %d\n", getpid());
    struct message m;
    read(fd, &m, sizeof(struct message));
    printf("Message received %s\n", m.actual_message);
}


int main()
{
    /*the pipe file descriptors*/
    int fd[2];
    /*create the pipe*/
    pipe(fd);

    if (fork()){
        /*process a will only send*/
        close(fd[0]);
        process_a(fd[1]);
        exit(1);
    }
    else{
        /*process b will only receive*/
        close(fd[1]);
        process_b(fd[0]);
        exit(1);
    }
}
