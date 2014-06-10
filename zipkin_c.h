#include <stdint.h>

//#define CHILD(child, info) blkin_init_child({}

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
    struct blkin_trace_info info;
    struct blkin_endpoint *trace_endpoint;
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
int blkin_init_new_trace(struct blkin_trace *new_trace, char *service, 
        struct blkin_endpoint *endpoint);

/* init child trace */
int blkin_init_child(struct blkin_trace *child, struct blkin_trace *parent, char *child_name);

/* create new endpoint */
int blkin_init_endpoint(struct blkin_endpoint * endp, char *ip, int port, 
        char *service_name);

/* create new string annotation */
int blkin_init_string_annotation(struct blkin_annotation *annotation, char *key, char *val, 
        struct blkin_endpoint * endpoint);

/* create new timestamp annotation */
int blkin_init_timestamp_annotation(struct blkin_annotation *annot, char *event, 
        struct blkin_endpoint * endpoint);

/* log the specific annotation for the specific trace */
int blkin_record(struct blkin_trace *trace, struct blkin_annotation *annotation);

/* Copy trace info to trace */
int blkin_get_trace_info(struct blkin_trace *trace, struct blkin_trace_info *info);

/* Set trace info */
int blkin_set_trace_info(struct blkin_trace *trace, struct blkin_trace_info *info);

/* Init child from trace_info */
int blkin_instant_child(struct blkin_trace *child, struct blkin_trace_info *info, char *child_name);
