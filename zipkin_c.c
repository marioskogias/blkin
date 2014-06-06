#include "zipkin_c.h"

#define TRACEPOINT_DEFINE
#include "zipkin_trace.h"

int64_t random_big()
{
    int64_t a;
    a = rand();
    a = a << 32;
    int b = rand();
    a = a + b;
    return a;
};

struct trace *new_trace(char *service, struct endpoint *endpoint) 
{
    struct trace *res;
    res = malloc(sizeof(struct trace));
    res->service = service;
    res->trace_id = random_big();
    res->span_id = random_big();
    res->parent_span_id = 0;
    res->trace_endpoint = endpoint;

    return res;
}

struct trace *child(struct trace *parent, char *child_name)
{
    struct trace *res;
    res = malloc(sizeof(struct trace));
    res->service = child_name;
    res->trace_id = parent->trace_id;
    res->span_id = random_big();
    res->parent_span_id = parent->span_id;
    res->trace_endpoint = parent->trace_endpoint;

    return res;
}

struct endpoint *new_endpoint(char *ip, int port, char *service_name)
{
    struct endpoint *res;
    res = malloc(sizeof(struct endpoint));
    res->ip = ip;
    res->port = port;
    res->service_name = service_name;
    return res;
}
struct annotation *string_annotation(char *key, char *val, 
        struct endpoint *endpoint)
{
    struct annotation *res;
    res = malloc(sizeof(struct annotation));
    res->type = ANNOT_STRING;
    res->key = key;
    res->val = val;
    res->annotation_endpoint = endpoint;
    return res;
}

struct annotation *timestamp_annotation(char *event, 
        struct endpoint *endpoint)
{
    struct annotation *res;
    res = malloc(sizeof(struct annotation));
    res->type = ANNOT_TIMESTAMP;
    res->val = event;
    res->annotation_endpoint = endpoint;
    return res;
}

void record(struct trace *trace, struct annotation *annotation)
{
    if (!annotation->annotation_endpoint && trace->trace_endpoint)
        annotation->annotation_endpoint = trace->trace_endpoint;
    
    if (annotation->type == ANNOT_STRING)
        tracepoint(zipkin, keyval , trace->service, 
                annotation->annotation_endpoint->port,
                trace->trace_id, trace->span_id, trace->parent_span_id, 
                annotation->key, annotation->val);
    else 
        tracepoint(zipkin, timestamp, trace->service, 
                annotation->annotation_endpoint->port,
                trace->trace_id, trace->span_id, trace->parent_span_id, 
                annotation->val);
}
