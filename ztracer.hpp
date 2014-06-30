#ifndef ZTRACER_H

#define ZTRACER_H

#include <sstream>
#include <boost/smart_ptr.hpp>

extern "C" {
#include <blkin-front.h>
}

using std::string;
using std::ostringstream;

namespace ZTracer {
	int ztrace_init(void);
	char * to_cstr(string &s);
	char * ostr_to_cstr(ostringstream &stream);

	class ZTraceEndpoint;
	class ZTrace;

	typedef boost::shared_ptr<ZTraceEndpoint> ZTraceEndpointRef;
	typedef boost::shared_ptr<ZTrace> ZTraceRef;

	class ZTraceEndpoint {
		private:
			struct blkin_endpoint ep;
			char *c_ip;
			char *c_name;
		protected:
			struct blkin_endpoint * get_blkin_ep()
			{
				return &ep;
			}
			friend ZTrace;
		public:
			ZTraceEndpoint(string ip, int port, string endpoint_name)
			{
				c_ip = to_cstr(ip);
				c_name = to_cstr(endpoint_name);
				blkin_init_endpoint(&ep, c_ip, port, c_name);
			}
			~ZTraceEndpoint()
			{
				//cout << "Endpoint destroyed" << std::endl;
				if (c_ip)
					delete [] c_ip;
				if (c_name)
					delete [] c_name;
			}
	};

	class ZTrace {
		private:
			struct blkin_trace trace;
			ZTraceEndpointRef ep;
			char *c_name = NULL;
		protected:
			struct blkin_trace *get_blkin_trace()
			{
				return &trace;
			}
		public:
			ZTrace(string name, ZTraceEndpointRef ep)
				:ep(ep)
			{
				c_name = to_cstr(name);
				blkin_init_new_trace(&trace, c_name, ep->get_blkin_ep());
			}

			ZTrace(string name, ZTraceRef t)
			{
				this->ep = t->ep;
				c_name = to_cstr(name);
				blkin_init_child(&trace, t->get_blkin_trace(), ep->get_blkin_ep(), c_name);
			}

			ZTrace(string name, ZTraceRef t, ZTraceEndpointRef ep)
			{
				this->ep = ep;
				c_name = to_cstr(name);
				blkin_init_child(&trace, t->get_blkin_trace(), ep->get_blkin_ep(),
						c_name);
			}

			ZTrace(string name, ZTraceEndpointRef ep, struct blkin_trace_info *info)
			{
				c_name = to_cstr(name);
				this->ep = ep;

				blkin_init_new_trace(&trace, c_name, ep->get_blkin_ep());
				blkin_set_trace_info(&trace, info);
			}
			~ZTrace()
			{
				if (c_name) {
					delete [] c_name;
				}
			}

			int get_trace_info(struct blkin_trace_info *info);
			int set_trace_info(struct blkin_trace_info *info);
			int keyval(string key, string val);
			int keyval(string key, string val, ZTraceEndpointRef ep);
			int event(string event);
			int event(string event, ZTraceEndpointRef ep);
	};

	ZTraceEndpointRef create_ZTraceEndpoint(string ip, int port, string name);
	ZTraceRef create_ZTrace(string name, ZTraceEndpointRef ep);
	ZTraceRef create_ZTrace(string name, ZTraceRef t);
	ZTraceRef create_ZTrace(string name, ZTraceRef t, ZTraceEndpointRef ep);
	ZTraceRef create_ZTrace(string name, ZTraceEndpointRef ep, struct blkin_trace_info *info);

}
#endif /* end of include guard: ZTRACER_H */
