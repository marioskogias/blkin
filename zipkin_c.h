#include <stdint.h>

struct endpoint {
    char * ip;
    int port;
    char * service_name;
};

struct trace {
    char * service;
    int64_t trace_id;
    int64_t span_id;
    int64_t parent_span_id;
    struct endpoint * trace_endpoint;
};

enum annotation_type {
    ANNOT_STRING = 0,
    ANNOT_TIMESTAMP
};

struct annotation {
    enum annotation_type type;
    char * key;
    char * val;
    struct endpoint * annotation_endpoint;
};

/* create new trace */
struct trace *new_trace(char * service, struct endpoint * endpoint);

/* create child trace */
struct trace *child(struct trace *parent, char *child_name);

/* create new endpoint */
struct endpoint *new_endpoint(char *ip, int port, char *service_name);

/* create new string annotation */
struct annotation *string_annotation(char *key, char *val, 
        struct endpoint * endpoint);

/* create new timestamp annotation */
struct annotation *timestamp_annotation(char *event, 
        struct endpoint * endpoint);

/* log the specific annotation for the specific trace */
void record(struct trace *trace, struct annotation *annotation);
