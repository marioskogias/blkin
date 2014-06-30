#include <ztracer.hpp>
#include <cstring>

namespace ZTracer {
	int ztrace_init(void)
	{
		return blkin_init();
	}

	char * to_cstr(string &s)
	{
		char *cstr = new char [s.length()+1];
		strcpy(cstr, s.c_str());

		return cstr;
	}

	char * ostr_to_cstr(ostringstream &stream)
	{
		string s;
		s = stream.str();

		return to_cstr(s);
	}
	int ZTrace::keyval(string key, string val)
	{
		BLKIN_KEYVAL(&trace, ep->get_blkin_ep(), (char *)key.c_str(),
				(char *)val.c_str());
		return 0;
	}

	int ZTrace::event(string event)
	{
		BLKIN_TIMESTAMP(&trace, ep->get_blkin_ep(), (char *)event.c_str());
		return 0;
	}

	int ZTrace::keyval(string key, string val, ZTraceEndpointRef ep)
	{
		BLKIN_KEYVAL(&trace, ep->get_blkin_ep(), (char *)key.c_str(),
				(char *)val.c_str());
		return 0;
	}

	int ZTrace::event(string event, ZTraceEndpointRef ep)
	{
		BLKIN_TIMESTAMP(&trace, ep->get_blkin_ep(), (char *)event.c_str());
		return 0;
	}


	ZTraceEndpointRef create_ZTraceEndpoint(string ip, int port, string name)
	{
		boost::shared_ptr<ZTraceEndpoint> ret(new ZTraceEndpoint(ip, port, name));
		return ret;
	}

	ZTraceRef create_ZTrace(string name, ZTraceEndpointRef ep)
	{
		boost::shared_ptr<ZTrace> ret(new ZTrace(name, ep));
		return ret;
	}

	ZTraceRef create_ZTrace(string name, ZTraceRef t)
	{
		boost::shared_ptr<ZTrace> ret(new ZTrace(name, t));
		return ret;
	}

	ZTraceRef create_ZTrace(string name, ZTraceRef t, ZTraceEndpointRef ep)
	{
		boost::shared_ptr<ZTrace> ret(new ZTrace(name, t, ep));
		return ret;
	}

	ZTraceRef create_ZTrace(string name, ZTraceEndpointRef ep, struct blkin_trace_info *info)
	{
		boost::shared_ptr<ZTrace> ret(new ZTrace(name, ep, info));
		return ret;
	}
	int ZTrace::get_trace_info(struct blkin_trace_info *info)
	{
		return blkin_get_trace_info(&trace, info);
	}
	int ZTrace::set_trace_info(struct blkin_trace_info *info)
	{
		return blkin_set_trace_info(&trace, info);
	}
}
