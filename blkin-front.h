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
#include <zipkin_c.h>

/* Define extern function pointers to be resolved during initialization */
extern int (*blkin_init_new_trace)(struct blkin_trace *new_trace, const char *name,
        struct blkin_endpoint *endpoint);

extern int (*blkin_init_child)(struct blkin_trace *child, struct blkin_trace *parent,
		struct blkin_endpoint *endpoint, const char *child_name);

extern int (*blkin_init_child_info)(struct blkin_trace *child,
        struct blkin_trace_info *info, struct blkin_endpoint *endpoint,
	const char *child_name);

extern int (*blkin_init_endpoint)(struct blkin_endpoint * endp, const char *ip,
		int port, const char *name);

extern int (*blkin_init_string_annotation)(struct blkin_annotation *annotation,
		const char *key, const char *val, struct blkin_endpoint * endpoint);

extern int (*blkin_init_timestamp_annotation)(struct blkin_annotation *annot,
		const char *event, struct blkin_endpoint * endpoint);

extern int (*blkin_record)(struct blkin_trace *trace,
		struct blkin_annotation *annotation);

extern int (*blkin_get_trace_info)(struct blkin_trace *trace,
	struct blkin_trace_info *info);

extern int (*blkin_set_trace_info)(struct blkin_trace *trace,
		struct blkin_trace_info *info);
extern int (*blkin_pack_trace_info)(struct blkin_trace_info *info,
				struct blkin_trace_info_packed *pinfo);

extern int (*blkin_unpack_trace_info)(struct blkin_trace_info_packed *pinfo,
				struct blkin_trace_info *info);

int blkin_init(void);
