/*
 * Copyright 2014 Marios Kogias <marioskogias@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 *   1. Redistributions of source code must retain the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer.
 *   2. Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "zipkin_c.h"
#include <errno.h>

#define TRACEPOINT_DEFINE
#include <zipkin_trace.h>

const char *default_ip = "NaN";
const char *default_name = "NoName";

int64_t random_big()
{
    int64_t a;
    a = rand();
    a = a << 32;
    int b = rand();
    a = a + b;
    if (a<0)
        a = !a;
    return a;
};

int _blkin_init_new_trace(struct blkin_trace *new_trace, const char *service,
        struct blkin_endpoint *endpoint)
{
    int res;
    if (!new_trace) {
        res = -EINVAL;
        goto OUT;
    }
    new_trace->name = service;
    new_trace->info.trace_id = random_big();
    new_trace->info.span_id = random_big();
    new_trace->info.parent_span_id = 0;
    new_trace->trace_endpoint = endpoint;
    res = 0;

OUT:
    return res;
}

int _blkin_init_child_info(struct blkin_trace *child,
        struct blkin_trace_info *parent_info, struct blkin_endpoint *endpoint,
	const char *child_name)
{
    int res;
    if ((!child) || (!parent_info) || (!endpoint)){
        res = -EINVAL;
        goto OUT;
    }
    child->info.trace_id = parent_info->trace_id;
    child->info.span_id = random_big();
    child->info.parent_span_id = parent_info->span_id;
    child->name = child_name;
    child->trace_endpoint = endpoint;
    res = 0;

OUT:
    return res;
}

int _blkin_init_child(struct blkin_trace *child, struct blkin_trace *parent,
		struct blkin_endpoint *endpoint, const char *child_name)
{
    int res;
    if (!parent) {
        res = -EINVAL;
        goto OUT;
    }
    if (!endpoint)
        endpoint = parent->trace_endpoint;
    if (_blkin_init_child_info(child, &parent->info, endpoint, child_name) != 0){
        res = -EINVAL;
        goto OUT;
    }
    res = 0;

OUT:
    return res;
}

int _blkin_init_endpoint(struct blkin_endpoint *endp, const char *ip, int port,
        const char *name)
{
    int res;
    if (!endp){
        res = -EINVAL;
        goto OUT;
    }
    if (!ip)
	    ip = default_ip;

    endp->ip = ip;
    endp->port = port;
    endp->name = name;
    res = 0;

OUT:
    return res;
}

int _blkin_init_string_annotation(struct blkin_annotation *annotation, const char *key,
        const char *val, struct blkin_endpoint *endpoint)
{
    int res;
    if ((!annotation) || (!key) || (!val)){
        res = -EINVAL;
        goto OUT;
    }
    annotation->type = ANNOT_STRING;
    annotation->key = key;
    annotation->val = val;
    annotation->annotation_endpoint = endpoint;
    res = 0;

OUT:
    return res;
}

int _blkin_init_timestamp_annotation(struct blkin_annotation *annotation,
        const char *event, struct blkin_endpoint *endpoint)
{
    int res;
    if ((!annotation) || (!event)){
        res = -EINVAL;
        goto OUT;
    }
    annotation->type = ANNOT_TIMESTAMP;
    annotation->val = event;
    annotation->annotation_endpoint = endpoint;
    res = 0;

OUT:
    return res;
}

int _blkin_record(struct blkin_trace *trace, struct blkin_annotation *annotation)
{
    int res;
    if ((!annotation) || (!trace)){
        res = -EINVAL;
        goto OUT;
    }
    if (!annotation->annotation_endpoint && trace->trace_endpoint)
        annotation->annotation_endpoint = trace->trace_endpoint;
    if (!trace->name) 
        trace->name = default_name;
    if (!annotation->annotation_endpoint->ip)
        annotation->annotation_endpoint->ip = default_ip;
    if (!annotation->annotation_endpoint->name)
        annotation->annotation_endpoint->name = default_name;

    if (annotation->type == ANNOT_STRING) {
        if ((!annotation->key) || (!annotation->val)) {
            res = -EINVAL;
            goto OUT;
        }
        tracepoint(zipkin, keyval, trace->name,
                annotation->annotation_endpoint->name,
                annotation->annotation_endpoint->port,
                annotation->annotation_endpoint->ip,
                trace->info.trace_id, trace->info.span_id,
                trace->info.parent_span_id,
                annotation->key, annotation->val);
    }
    else {
        if (!annotation->val) {
            res = -EINVAL;
            goto OUT;
        }
        tracepoint(zipkin, timestamp , trace->name,
                annotation->annotation_endpoint->name,
                annotation->annotation_endpoint->port,
                annotation->annotation_endpoint->ip,
                trace->info.trace_id, trace->info.span_id,
                trace->info.parent_span_id,
                annotation->val);
    }
    res = 0;
OUT:
    return res;
}

int _blkin_get_trace_info(struct blkin_trace *trace,
        struct blkin_trace_info *info)
{
    int res;
    if ((!trace) || (!info)){
        res = -EINVAL;
        goto OUT;
    }

    *info = trace->info;
OUT:
    return res;
}

int _blkin_set_trace_info(struct blkin_trace *trace,
        struct blkin_trace_info *info)
{
    int res;
    if ((!trace) || (!info)){
        res = -EINVAL;
        goto OUT;
    }
    
    trace->info = *info;

OUT:
    return res;
}

int _blkin_pack_trace_info(struct blkin_trace_info *info,
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

int _blkin_unpack_trace_info(struct blkin_trace_info_packed *pinfo,
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
