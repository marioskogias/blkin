#define SOCK_PATH "/var/run/blkin/blkin.sock"

struct sampling_info {
    int rate;
    int req_count;
};

struct sampling_info *sampling_init();

struct sampling_info *p_sinfo;
