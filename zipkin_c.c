#include "zipkin_c.h"
#include <errno.h>

#define TRACEPOINT_DEFINE
#include "zipkin_trace.h"

const char *default_ip = "NaN";

int64_t random_big()
{
    int64_t a;
    a = rand();
    a = a << 32;
    int b = rand();
    a = a + b;
    return a;
};

int blkin_init_new_trace(struct blkin_trace *new_trace, char *service,
        struct blkin_endpoint *endpoint)
{
    int res;
    if (!new_trace) {
        res = -1;
        goto OUT;
    }
    new_trace->name = service;
    new_trace->info.trace_id = random_big();
    new_trace->info.span_id = random_big();
    new_trace->info.parent_span_id = 0;
    new_trace->trace_endpoint = endpoint;
    res = 1;

OUT:
    return res;
}

int blkin_init_child_info(struct blkin_trace *child,
        struct blkin_trace_info *parent_info, struct blkin_endpoint *endpoint,
	char *child_name)
{
    int res;
    if ((!child) || (!parent_info)){
        res = -1;
        goto OUT;
    }
    child->info.trace_id = parent_info->trace_id;
    child->info.span_id = random_big();
    child->info.parent_span_id = parent_info->span_id;
    child->name = child_name;
    child->trace_endpoint = endpoint;
    res = 1;

OUT:
    return res;
}

int blkin_init_child(struct blkin_trace *child, struct blkin_trace *parent,
		struct blkin_endpoint *endpoint, char *child_name)
{
    int res;
    if (!parent) {
        res = -1;
        goto OUT;
    }
    if (!endpoint)
        endpoint = parent->trace_endpoint;
    if (!blkin_init_child_info(child, &parent->info, endpoint, child_name)){
        res = -1;
        goto OUT;
    }
    res = 1;

OUT:
    return res;
}

int blkin_init_endpoint(struct blkin_endpoint *endp, char *ip, int port,
        char *service_name)
{
    int res;
    if (!endp){
        res = -1;
        goto OUT;
    }
    if (!ip)
	    ip = default_ip;

    endp->ip = ip;
    endp->port = port;
    endp->service_name = service_name;
    res = 1;

OUT:
    return res;
}

int blkin_init_string_annotation(struct blkin_annotation *annotation, char *key,
        char *val, struct blkin_endpoint *endpoint)
{
    int res;
    if (!annotation){
        res = -1;
        goto OUT;
    }
    annotation->type = ANNOT_STRING;
    annotation->key = key;
    annotation->val = val;
    annotation->annotation_endpoint = endpoint;
    res = 1;

OUT:
    return res;
}

int blkin_init_timestamp_annotation(struct blkin_annotation *annotation,
        char *event, struct blkin_endpoint *endpoint)
{
    int res;
    if (!annotation){
        res = -1;
        goto OUT;
    }
    annotation->type = ANNOT_TIMESTAMP;
    annotation->val = event;
    annotation->annotation_endpoint = endpoint;
    res = 1;

OUT:
    return res;
}

int blkin_record(struct blkin_trace *trace, struct blkin_annotation *annotation)
{
    int res;
    if ((!annotation) || (!trace)){
        res = -1;
        goto OUT;
    }
    if (!annotation->annotation_endpoint && trace->trace_endpoint)
        annotation->annotation_endpoint = trace->trace_endpoint;

    if (annotation->type == ANNOT_STRING)
        tracepoint(zipkin, keyval, trace->name,
                annotation->annotation_endpoint->service_name,
                annotation->annotation_endpoint->port,
                annotation->annotation_endpoint->ip,
                trace->info.trace_id, trace->info.span_id,
                trace->info.parent_span_id,
                annotation->key, annotation->val);
    else
        tracepoint(zipkin, timestamp , trace->name,
                annotation->annotation_endpoint->service_name,
                annotation->annotation_endpoint->port,
                annotation->annotation_endpoint->ip,
                trace->info.trace_id, trace->info.span_id,
                trace->info.parent_span_id,
                annotation->val);
    res = 1;
OUT:
    return res;
}

int blkin_get_trace_info(struct blkin_trace *trace,
        struct blkin_trace_info *info)
{
    int res;
    if ((!trace) || (!info)){
        res = -1;
        goto OUT;
    }

    memcpy(&trace->info, info, sizeof(struct blkin_trace_info));

OUT:
    return res;
}

int blkin_set_trace_info(struct blkin_trace *trace,
        struct blkin_trace_info *info)
{
    int res;
    if ((!trace) || (!info)){
        res = -1;
        goto OUT;
    }

    memcpy(info, &trace->info, sizeof(struct blkin_trace_info));

OUT:
    return res;
}

int blkin_instant_child(struct blkin_trace *child,
        struct blkin_trace_info *info, char *child_name)
{
    //Is this possible with a macro?
    struct blkin_trace parent;
    blkin_get_trace_info(&parent, info);
    return blkin_init_child(child, &parent, NULL, child_name);
}

int blkin_pack_trace_info(struct blkin_trace_info *info,
				struct blkin_trace_info_packed *pinfo)
{
	if (!info || !pinfo) {
		return -EINVAL;
	}

	info->trace_id = __be64_to_cpu(pinfo->trace_id);
	info->span_id = __be64_to_cpu(pinfo->span_id);
	info->parent_span_id = __be64_to_cpu(pinfo->parent_span_id);

	return 1;
}

int blkin_unpack_trace_info(struct blkin_trace_info_packed *pinfo,
				struct blkin_trace_info *info)
{
	if (!info || !pinfo) {
		return -EINVAL;
	}

	pinfo->trace_id = __cpu_to_be64(info->trace_id);
	pinfo->span_id = __cpu_to_be64(info->span_id);
	pinfo->parent_span_id = __cpu_to_be64(info->parent_span_id);

	return 1;
}


