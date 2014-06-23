#include <zipkin_c.h>

/* Define extern function pointers to be resolved during initialization */
extern int (*blkin_init_new_trace)(struct blkin_trace *new_trace, char *name,
        struct blkin_endpoint *endpoint);

extern int (*blkin_init_child)(struct blkin_trace *child, struct blkin_trace *parent,
		struct blkin_endpoint *endpoint, char *child_name);

extern int (*blkin_init_child_info)(struct blkin_trace *child,
        struct blkin_trace_info *info, struct blkin_endpoint *endpoint,
	char *child_name);

extern int (*blkin_init_endpoint)(struct blkin_endpoint * endp, char *ip,
		int port, char *name);

extern int (*blkin_init_string_annotation)(struct blkin_annotation *annotation,
		char *key, char *val, struct blkin_endpoint * endpoint);

extern int (*blkin_init_timestamp_annotation)(struct blkin_annotation *annot,
		char *event, struct blkin_endpoint * endpoint);

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
