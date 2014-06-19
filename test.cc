#include <ztracer.hpp>

int main(int argc, const char *argv[])
{
	ZTracer::ZTraceEndpointRef t = ZTracer::create_ZTraceEndpoint("foo", 0, "fadfs");
	return 0;
}
