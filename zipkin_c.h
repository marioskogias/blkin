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
#include <stdint.h>
#include <asm/byteorder.h>

#define BLKIN_TIMESTAMP(trace, endp, event) 				\
	do {								\
		struct blkin_annotation __annot;			\
		blkin_init_timestamp_annotation(&__annot, event, endp); \
		blkin_record(trace, &__annot);				\
	} while (0);

#define BLKIN_KEYVAL(trace, endp, key, val) 				\
	do {								\
		struct blkin_annotation __annot;			\
		blkin_init_string_annotation(&__annot, key, val, endp);	\
		blkin_record(trace, &__annot);				\
	} while (0);

/**
 * @struct blkin_endpoint
 * Information about an endpoint of our instrumented application where 
 * annotations take place
 */ 
struct blkin_endpoint {
    const char *ip;
    int port;
    const char *name;
};

/**
 * @struct blkin_trace_info
 * The information exchanged between different layers offering the needed 
 * trace semantics
 */
struct blkin_trace_info {
    int64_t trace_id;
    int64_t span_id;
    int64_t parent_span_id;
};

/**
 * @struct blkin_trace_info_packed
 *
 * Packed version of the struct blkin_trace_info. Usefull when sending over
 * network.
 *
 */
struct blkin_trace_info_packed {
	__be64 trace_id;
	__be64 span_id;
	__be64 parent_span_id;
} __attribute__((packed));


/**
 * @struct blkin_trace
 * Struct used to define the context in which an annotation happens
 */
struct blkin_trace {
    const char *name;
    struct blkin_trace_info info;
    struct blkin_endpoint *trace_endpoint;
};

/**
 * @typedef blkin_annotation_type
 * There are 2 kinds of annotation key-val and timestamp
 */
typedef enum {
    ANNOT_STRING = 0,
    ANNOT_TIMESTAMP
} blkin_annotation_type;

/**
 * @struct blkin_annotation
 * Struct carrying information about an annotation. This information can either
 * be key-val or that a specific event happened
 */
struct blkin_annotation {
    blkin_annotation_type type;
    const char *key;
    const char *val;
    struct blkin_endpoint *annotation_endpoint;
};

#if 0
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
int _blkin_init_new_trace(struct blkin_trace *new_trace, char *name,
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
int _blkin_init_child(struct blkin_trace *child, struct blkin_trace *parent,
		struct blkin_endpoint *endpoint,
		char *child_name);

/**
 * Initialize a blkin_trace struct and set the blkin_trace_info field to be 
 * child of the given blkin_trace_info. This means
 * Same trace_id
 * Different span_id
 * Child's parent_span_id == parent's span_id
 *
 * @param child the new child blkin_trace_info
 * @param info the parent's blkin_trace_info struct
 * @param child_name the blkin_trace struct name field
 *
 * @returns 1 if success -1 if error
 */
int _blkin_init_child_info(struct blkin_trace *child,
        struct blkin_trace_info *info, struct blkin_endpoint *endpoint,
	char *child_name);

/**
 * Initialize a blkin_endpoint struct with the information given
 *
 * @param endp the endpoint to be initialized
 * @param ip the ip address of the specific endpoint
 * @param port the TCP/UDP port of the specific endpoint
 * @param name the name of the service running on the specific endpoint
 *
 * @returns 1 if success -1 if error
 */
int _blkin_init_endpoint(struct blkin_endpoint * endp, char *ip, int port, 
		char *name);

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
int _blkin_init_string_annotation(struct blkin_annotation *annotation, char *key,
		char *val,
		struct blkin_endpoint * endpoint);

/**
 * Initialize a timestamp blkin_annotation
 *
 * @param annotation the annotation to be initialized
 * @param event the event happened to be annotated
 * @param endpoint where did this annotation occured
 *
 * @returns 1 if success -1 if error
 */
int _blkin_init_timestamp_annotation(struct blkin_annotation *annot, char *event,
        struct blkin_endpoint * endpoint);

/**
 * Log an annotation in terms of a specific trace
 *
 * @param trace the trace to which the annotation belongs
 * @param annotation the annotation to be logged
 *
 * @returns 1 if success -1 if error
 */
int _blkin_record(struct blkin_trace *trace,
		struct blkin_annotation *annotation);

/**
 * Copy a blkin_trace_info struct into a the field info of a blkin_trace struct
 *
 * @param trace the destination
 * @param info where to copy from
 *
 * @returns 1 if success -1 if error
 */
int _blkin_get_trace_info(struct blkin_trace *trace,
        struct blkin_trace_info *info);

/**
 * Copy the blkin_trace_info from a blkin_trace to another blkin_trace_info
 *
 * @param trace the trace with the essential info
 * @param info the destination
 *
 * @returns 1 if success -1 if error
 */
int _blkin_set_trace_info(struct blkin_trace *trace,
		struct blkin_trace_info *info);


/**
 * Convert a blkin_trace_info to the packed version.
 *
 * @param info The unpacked trace info.
 * @param pinfo The provided packed version to be initialized.
 *
 * @returns 1 on success, -1 on error
 */
int _blkin_pack_trace_info(struct blkin_trace_info *info,
		struct blkin_trace_info_packed *pinfo);

/**
 * Convert a packed blkin_trace_info to the unpacked version.
 *
 * @param pinfo The provided packed version to be unpacked.
 * @param info The unpacked trace info.
 *
 * @returns 1 on success, -1 on error
 */
int _blkin_unpack_trace_info(struct blkin_trace_info_packed *pinfo,
			struct blkin_trace_info *info);

#endif
