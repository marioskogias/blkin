/*
 * Zipkin lttng-ust tracepoint provider. 
 */

#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER zipkin

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./zipkin_trace.h"

#if !defined(_ZIPKIN_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _ZIPKIN_H

#include <lttng/tracepoint.h> 

TRACEPOINT_EVENT(
        zipkin,
        keyval,
        TP_ARGS(const char *, trace_name, const char *, service, 
            int, port, const char *, ip, long, trace, 
            long, span, long, parent_span, 
            const char *, key, const char *, val ),
        
        TP_FIELDS(
                /*
                 * Each span has a name mentioned on it in the UI
                 * This is the trace name 
                 */
                ctf_string(trace_name, trace_name)
                /*
                 * Each trace takes place in a specific machine-endpoint
                 * This is identified by a name, a port number and an ip
                 */
                ctf_string(service_name, service)
                ctf_integer(int, port_no, port)
                ctf_string(ip, ip)
                /*
                 * According to the tracing semantics each trace should have
                 * a trace id, a span id and a parent span id 
                 */
                ctf_integer(long, trace_id, trace)
                ctf_integer(long, span_id, span)
                ctf_integer(long, parent_span_id, parent_span)
                /*
                 * The following is the real annotated information
                 */
                ctf_string(key, key)
                ctf_string(val, val)
        ) 
)
TRACEPOINT_LOGLEVEL(
        zipkin, 
        keyval, 
        TRACE_WARNING)

/*
 * In this event we follow the same semantics but we trace timestamp annotations
 */

TRACEPOINT_EVENT(
        zipkin,
        timestamp,
        TP_ARGS(const char *, trace_name, const char *, service, 
            int, port, const char *, ip, long, trace, 
            long, span, long, parent_span, 
            const char *, event),
        
        TP_FIELDS(
                ctf_string(trace_name, trace_name)
                ctf_string(service_name, service)
                ctf_integer(int, port_no, port)
                ctf_string(ip, ip)
                ctf_integer(long, trace_id, trace)
                ctf_integer(long, span_id, span)
                ctf_integer(long, parent_span_id, parent_span)
                ctf_string(event, event)
        ) 
)
TRACEPOINT_LOGLEVEL(
        zipkin, 
        timestamp, 
        TRACE_WARNING)
#endif /* _ZIPKIN_H */

#include <lttng/tracepoint-event.h>

