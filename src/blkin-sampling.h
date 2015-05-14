#define SOCK_PATH "/var/run/blkin/blkin.sock"

#define BLKIN_ENROLL 1
#define BLKIN_LEAVE 0

/*This struct is sent to the blkin-janitor to enroll or leave adaptive tracing*/
struct connection_info {
    int pid;
    int action;
};

/*This info in on the shm segment*/
struct sampling_info {
    int rate;
    int req_count;
};

struct sampling_info *sampling_init();

struct sampling_info *p_sinfo;
