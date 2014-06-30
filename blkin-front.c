#include <blkin-front.h>
#include <dlfcn.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sys/types.h>
#include <unistd.h>
                            
/* Function pointers to be resolved during initialization */
int (*blkin_init_new_trace)(struct blkin_trace *new_trace, char *name,
        struct blkin_endpoint *endpoint);

int (*blkin_init_child)(struct blkin_trace *child, struct blkin_trace *parent,
		struct blkin_endpoint *endpoint, char *child_name);

int (*blkin_init_child_info)(struct blkin_trace *child,
        struct blkin_trace_info *info, struct blkin_endpoint *endpoint,
	char *child_name);

int (*blkin_init_endpoint)(struct blkin_endpoint * endp, char *ip,
		int port, char *name);

int (*blkin_init_string_annotation)(struct blkin_annotation *annotation,
		char *key, char *val, struct blkin_endpoint * endpoint);

int (*blkin_init_timestamp_annotation)(struct blkin_annotation *annot,
		char *event, struct blkin_endpoint * endpoint);

int (*blkin_record)(struct blkin_trace *trace,
		struct blkin_annotation *annotation);

int (*blkin_get_trace_info)(struct blkin_trace *trace,
	struct blkin_trace_info *info);

int (*blkin_set_trace_info)(struct blkin_trace *trace,
		struct blkin_trace_info *info);
int (*blkin_pack_trace_info)(struct blkin_trace_info *info,
				struct blkin_trace_info_packed *pinfo);

int (*blkin_unpack_trace_info)(struct blkin_trace_info_packed *pinfo,
				struct blkin_trace_info *info);

static int resolve_symbols()
{
	void *handle;
	char *error;

	handle = dlopen("libzipkin-c.so", RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		return -ENOENT;
	}
	dlerror();

	*(void **) (&blkin_init_new_trace) = dlsym(handle, "_blkin_init_new_trace");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_init_child) = dlsym(handle, "_blkin_init_child");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_init_child_info) = dlsym(handle, "_blkin_init_child_info");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_init_endpoint) = dlsym(handle, "_blkin_init_endpoint");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_init_string_annotation) = dlsym(handle, "_blkin_init_string_annotation");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_init_timestamp_annotation) = dlsym(handle, "_blkin_init_timestamp_annotation");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_record) = dlsym(handle, "_blkin_record");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_set_trace_info) = dlsym(handle, "_blkin_set_trace_info");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_get_trace_info) = dlsym(handle, "_blkin_get_trace_info");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_pack_trace_info) = dlsym(handle, "_blkin_pack_trace_info");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	*(void **) (&blkin_unpack_trace_info) = dlsym(handle, "_blkin_unpack_trace_info");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		return -ENXIO;
	}

	return 0;
}

static pthread_mutex_t blkin_init_mutex = PTHREAD_MUTEX_INITIALIZER;
static int initialized = 0;

int blkin_init(void)
{
    /*
     * Initialize srand with sth appropriete
     * time is not good for archipelago: several deamons -> same timstamp
     */
    srand(getpid()*time(NULL));
	int ret;
	pthread_mutex_lock(&blkin_init_mutex);
	if (!initialized) {
		ret = resolve_symbols();
		if (ret >= 0) {
			initialized = 1;
		}
	}
	pthread_mutex_unlock(&blkin_init_mutex);
	return ret;

}
