#include <stdint.h>

struct blkin_endpoint {
    char *ip;
    int port;
    char *service_name;
};

struct blkin_trace_info {
    int64_t trace_id;
    int64_t span_id;
    int64_t parent_span_id;
};

struct blkin_trace {
    char *service;
    struct trace_info info;
    struct endpoint *trace_endpoint;
};

typedef enum {
    ANNOT_STRING = 0,
    ANNOT_TIMESTAMP
} blkin_annotation_type;

struct blkin_annotation {
    blkin_annotation_type type;
    char *key;
    char *val;
    struct blkin_endpoint *annotation_endpoint;
};

/* init new trace */
int init_new_trace(struct trace *new_trace, char *service, 
        struct endpoint *endpoint);

/* init child trace */
int init_child(struct trace *child, struct trace *parent, char *child_name);

/* create new endpoint */
int init_endpoint(struct endpoint * entp, char *ip, int port, 
        char *service_name);

/* create new string annotation */
int init_string_annotation(struct annotation *annot, char *key, char *val, 
        struct endpoint * endpoint);

/* create new timestamp annotation */
int init_timestamp_annotation(struct annotation *annot, char *event, 
        struct endpoint * endpoint);

/* log the specific annotation for the specific trace */
void record(struct trace *trace, struct annotation *annotation);
