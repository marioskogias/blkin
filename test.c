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
    int i;
    printf("I am process A: %d\n", getpid());
    
    /*initialize endpoint*/
    struct blkin_endpoint endp;
    blkin_init_endpoint(&endp, "10.0.0.1", 5000, "service a");

    struct blkin_trace trace;
    struct blkin_annotation ant;
    struct message msg = {.actual_message = "message"};
    
    for (i=0;i<10;i++) {
        
        /*create trace*/
        blkin_init_new_trace(&trace, "process a", &endp);
        
        blkin_init_timestamp_annotation(&ant, "start", &endp);
        blkin_record(&trace, &ant);
        
        /*set trace fields to message*/
        blkin_set_trace_info(&trace, &msg.trace_info);
        
        /*send*/
        write(fd, &msg, sizeof(struct message));
        
        /*create annotation and log*/
        blkin_init_timestamp_annotation(&ant, "end", &endp);
        blkin_record(&trace, &ant);
    }
}

void process_b(int fd) 
{
    int i;
    printf("I am process B: %d\n", getpid());
    
    /*initialize endpoint*/
    struct blkin_endpoint endp;
    blkin_init_endpoint(&endp, "10.0.0.1", 5000, "service b");
    
    struct blkin_trace trace;
    struct blkin_annotation ant;
    struct message msg;

    for (i=0;i<10;i++) {
        read(fd, &msg, sizeof(struct message));
        
        /*create child trace*/
        blkin_instant_child(&trace, &msg.trace_info, "process b");  
        
        /*create annotation and log*/
        blkin_init_timestamp_annotation(&ant, "start", &endp);
        blkin_record(&trace, &ant);
        
        /*Process...*/
        usleep(10);
        printf("Message received %s\n", msg.actual_message);
        
        /*create annotation and log*/
        blkin_init_timestamp_annotation(&ant, "start", &endp);
        blkin_record(&trace, &ant);

    }
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
