#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <blkin-sampling.h>

#define INTERVAL 1 // time interval in seconds
#define REC_REQ 100 // required requests in INTERVAL

struct list_el {
    struct sampling_info * p;
    int owner_pid;
    struct list_el * next;
};

struct list_el *info = NULL;

void release_shm(int pid)
{
    struct list_el *p;
    struct list_el * p_prev = info;

    if (p_prev == NULL)
        return;
    p = p_prev->next;

    while (p!=NULL) {
        if (p->owner_pid == pid) {
            p_prev->next = p->next;
            free(p); 
            return;
        }
        p_prev = p;
        p = p->next;
    }

}

struct sampling_info *get_shm(key_t key)
{
    int shmid;
    struct sampling_info *data;

    if ((shmid = shmget(key, sizeof(struct sampling_info), 00700)) == -1) {
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

/* this function is run by the second thread */
void *listen_for_incoming()
{
    printf("I am listening for incoming calls\n");
    
    /*create and bind socket*/
    int s, s2, t, len;
    struct sockaddr_un local, remote;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Waiting for a connection...\n");
    t = sizeof(remote);
    struct connection_info cinfo;  
    struct list_el *el; 
    
    while(1) {
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
            perror("accept");
            exit(1);
        }
        recv(s2, &cinfo, sizeof(struct connection_info), 0);
        printf("I received the pid %d and action %d\n", cinfo.pid, cinfo.action);
        if (cinfo.action == BLKIN_ENROLL) {
            el = malloc(sizeof(struct list_el *));
            el->p = get_shm(cinfo.pid);
            el->next = info;
            el->owner_pid = cinfo.pid;
            info = el;
        } else if (cinfo.action == BLKIN_LEAVE) 
            release_shm(cinfo.pid);
    }

    return NULL;
}

void compute()
{
    struct list_el *p = info;
    int sum = 0;

    /*First calculate the request sum*/
    int i = 0;
    while (p!=NULL) {
        sum += p->p->req_count;
        printf("Process %d count = %d \n",i++, p->p->req_count);
        p = p->next;
    }
    printf("I computed the sum = %d\n", sum);

    /*Calculate and set the new rates - zero the req_counts*/
    p = info;
    int product;
    while (p!=NULL) {
        product = p->p->req_count ? REC_REQ * p->p->req_count : 1;
        p->p->rate = (sum*sum) / product;
        p->p->req_count = 0;
        p = p->next;
    }
}

 
int main()
{

	/* this variable is our reference to the second thread */
	pthread_t listener_thread;
    
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&listener_thread, NULL, listen_for_incoming, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

    printf("I will do the computations\n");
	while(1) {
        sleep(INTERVAL);
        compute();
    }
    /* wait for the second thread to finish */
	if(pthread_join(listener_thread, NULL)) {

		fprintf(stderr, "Error joining thread\n");
		return 2;

	}


	return 0;

}
