#!/usr/bin/python
# babeltrace_zipkin.py

import sys
#sys.path.append("../../babeltrace-plugins")
sys.path.append("zipkin_logic")
sys.path.append("zipkin_logic/facebook_scribe_python3")
import sys
import getopt
from babeltrace import *
from zipkin_logic.zipkin_client import ZipkinClient
HELP = "Usage: python babeltrace_zipkin.py path/to/file -s <server> -p <port>"


def main(argv):
    try:
        path = argv[0]
    except:
        raise TypeError(HELP)

    try:
        opts, args = getopt.getopt(argv[1:], "hs:p:")
    except getopt.GetoptError:
        raise TypeError(HELP)

    server = None
    port = None
    for opt, arg in opts:
        if opt == '-h':
            raise TypeError(HELP)
        elif opt == '-s':
            server = arg
        elif opt == '-p':
            port = arg

    if not server:
        server = "127.0.0.1"
    if not port:
        port = 9410

    # Open connection with scribe
    zipkin = ZipkinClient(port,  server)

    # Create TraceCollection and add trace:
    traces = TraceCollection()
    print(path)
    trace_handle = traces.add_trace(path, "ctf")
    if trace_handle is None:
        raise IOError("Error adding trace")

    for event in traces.events:
        name = event.name
        try:
            provider, kind = name.split(":")
            if provider != "zipkin":
                raise
        except:
            continue

        #create a zipkin trace from event info
        try:
            trace = zipkin.create_trace(event)
        except:
            continue

        #create a zipkin annotation from event info
        try:
            annotation = zipkin.create_annotation(event, kind)
        except:
            continue

        #record the trace
        zipkin.record(trace, annotation)

    zipkin.close()

if __name__ == "__main__":
    main(sys.argv[1:])
