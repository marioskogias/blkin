#include <unistd.h>
#include "zipkin_c.h"

void child_process(struct trace *parent) 
{
    struct trace * child_trace = child(parent, "child_service");
    struct endpoint *child_endpoint = new_endpoint("10.0.0.2", 1002, "child");
    struct annotation * child_an = timestamp_annotation("child_start", child_endpoint);

    record(child_trace, child_an);
    usleep(10);
    child_an = string_annotation("key", "val", child_endpoint);
    record(child_trace, child_an);
    usleep(10);
    child_an = timestamp_annotation("child_end", child_endpoint);
    record(child_trace, child_an);
}

int main() {
    struct endpoint *parent_endpoint = new_endpoint("10.0.0.1",1000,"parent");
    struct trace *p_trace = new_trace("parent_service", parent_endpoint);

    struct annotation *parent_an = timestamp_annotation("parent start", NULL);
    record(p_trace, parent_an);
    child_process(p_trace);
    usleep(100);
    parent_an = timestamp_annotation("parent end", NULL);
    record(p_trace, parent_an);
}
