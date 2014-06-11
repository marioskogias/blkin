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
    char *name;
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

/**
 * Initialize a new blkin_trace with the information given. The new trace will 
 * have no parent so the parent id will be zero.
 *
 * @param new_trace the blkin_trace to be initialized
 * @param name the trace's name 
 * @param endpoint a pointer to a blkin_endpoint struct that contains info about
 * where the specif trace takes place
 *
 * @returns 1 if success -1 if error
 */
int blkin_init_new_trace(struct blkin_trace *new_trace, char *name, 
        struct blkin_endpoint *endpoint);

/**
 * Initialize a blkin_trace as a child of the given parent bkin_trace. The child
 *  trace will have the same trace_id, new span_id and parent_span_id its 
 *  parent's span_id.
 *
 * @param child the blkin_trace to be initialized
 * @param parent the parent blkin_trace 
 * @param child_name the blkin_trace name of the child
 *
 * @returns 1 if success -1 if error
 */
int blkin_init_child(struct blkin_trace *child, struct blkin_trace *parent, 
        char *child_name);

/**
 * Initialize a blkin_trace_info struct as child of another given one. This means
 * Same trace_id
 * Different span_id
 * Child's parent_span_id == parent's span_id
 *
 * @param child the new child blkin_trace_info
 * @param parent the parent blkin_trace_info to get the information from
 *
 * @returns 1 if success -1 if error
 */
int blkin_init_child_info(struct bkin_trace_info *child, 
        struct blkin_trace_info *parent);

/**
 * Initialize a blkin_trace struct and set the blkin_trace_info field to be 
 * child of the given blkin_trace_info
 *
 * @param child the new child blkin_trace_info
 * @param info the parent's blkin_trace_info struct
 * @param child_name the blkin_trace struct name field
 *
 * @returns 1 if success -1 if error
 */
int blkin_init_child_with_info(struct blkin_trace *child,
        struct blkin_trace_info *info, char *child_name);

/**
 * Initialize a blkin_endpoint struct with the information given
 *
 * @param endp the endpoint to be initialized
 * @param ip the ip address of the specific endpoint
 * @param port the TCP/UDP port of the specific endpoint
 * @param service_name the name of the service running on the specific endpoint
 *
 * @returns 1 if success -1 if error
 */
int blkin_init_endpoint(struct blkin_endpoint * endp, char *ip, int port, 
        char *service_name);

/**
 * Initialize a key-value blkin_annotation
 *
 * @param annotation the annotation to be initialized
 * @param key the annotation's key
 * @param val the annotation's value
 * @param endpoint where did this annotation occured
 *
 * @returns 1 if success -1 if error
 */
int blkin_init_string_annotation(struct blkin_annotation *annotation, char *key,
        char *val, struct blkin_endpoint * endpoint);

/**
 * Initialize a timestamp blkin_annotation
 *
 * @param annotation the annotation to be initialized
 * @param event the event happened to be annotated
 * @param endpoint where did this annotation occured
 *
 * @returns 1 if success -1 if error
 */
int blkin_init_timestamp_annotation(struct blkin_annotation *annot, char *event, 
        struct blkin_endpoint * endpoint);

/**
 * Log an annotation in terms of a specific trace
 *
 * @param trace the trace to which the annotation belongs
 * @param annotation the annotation to be logged
 *
 * @returns 1 if success -1 if error
 */
int blkin_record(struct blkin_trace *trace, struct blkin_annotation *annotation);

/**
 * Copy a blkin_trace_info struct into a the field info of a blkin_trace struct
 *
 * @param trace the destination
 * @param info where to copy from
 *
 * @returns 1 if success -1 if error
 */
int blkin_get_trace_info(struct blkin_trace *trace,
        struct blkin_trace_info *info);

/**
 * Copy the blkin_trace_info from a blkin_trace to another blkin_trace_info
 *
 * @param trace the trace with the essential info
 * @param info the destination
 *
 * @returns 1 if success -1 if error
 */
int blkin_set_trace_info(struct blkin_trace *trace, struct blkin_trace_info *info);
