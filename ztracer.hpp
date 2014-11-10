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
#ifndef ZTRACER_H

#define ZTRACER_H

#include <sstream>
#include <boost/smart_ptr.hpp>

extern "C" {
#include <blkin-front.h>
}

namespace ZTracer {
	using std::string;

	int ztrace_init(void);

	class ZTraceEndpoint;
	class ZTrace;

	typedef boost::shared_ptr<ZTraceEndpoint> ZTraceEndpointRef;
	typedef boost::shared_ptr<ZTrace> ZTraceRef;

	class ZTraceEndpoint {
		private:
			struct blkin_endpoint ep;
			string ip;
			string name;

			struct blkin_endpoint * get_blkin_ep()
			{
				return &ep;
			}
			friend ZTrace;
		public:
			ZTraceEndpoint(const string &ip, int port, const string &endpoint_name)
				: ip(ip), name(endpoint_name)
			{
				blkin_init_endpoint(&ep, ip.c_str(), port, name.c_str());
			}
			~ZTraceEndpoint()
			{
				//cout << "Endpoint destroyed" << std::endl;
			}
	};

	class ZTrace {
		private:
			struct blkin_trace trace;
			ZTraceEndpointRef ep;
			string name;

			struct blkin_trace *get_blkin_trace()
			{
				return &trace;
			}
		public:
			ZTrace(const string &name, ZTraceEndpointRef ep)
				: ep(ep), name(name)
			{
				blkin_init_new_trace(&trace, name.c_str(), ep->get_blkin_ep());
			}

			ZTrace(const string &name, ZTraceRef t)
				: ep(t->ep), name(name)
			{
				blkin_init_child(&trace, t->get_blkin_trace(),
						ep->get_blkin_ep(), name.c_str());
			}

			ZTrace(const string &name, ZTraceRef t, ZTraceEndpointRef ep)
				: ep(ep), name(name)
			{
				blkin_init_child(&trace, t->get_blkin_trace(), ep->get_blkin_ep(),
						name.c_str());
			}

			ZTrace(const string &name, ZTraceEndpointRef ep, struct blkin_trace_info *info, bool child=false)
				: ep(ep), name(name)
			{
				if (child)
					blkin_init_child_info(&trace, info, ep->get_blkin_ep(), name.c_str());
				else {
					blkin_init_new_trace(&trace, name.c_str(), ep->get_blkin_ep());
					blkin_set_trace_info(&trace, info);
				}
			}

            ZTraceEndpointRef get_endpoint()
            {
                return this->ep;
            }

			int get_trace_info(struct blkin_trace_info *info);
			int set_trace_info(struct blkin_trace_info *info);
			int keyval(const string &key, const string &val);
			int keyval(const string &key, const string &val, ZTraceEndpointRef ep);
			int event(const string &event);
			int event(const string &event, ZTraceEndpointRef ep);
	};

	ZTraceEndpointRef create_ZTraceEndpoint(const string &ip, int port, const string &name);
	ZTraceRef create_ZTrace(const string &name, ZTraceEndpointRef ep);
	ZTraceRef create_ZTrace(const string &name, ZTraceRef t);
	ZTraceRef create_ZTrace(const string &name, ZTraceRef t, ZTraceEndpointRef ep);
	ZTraceRef create_ZTrace(const string &name, ZTraceEndpointRef ep, struct blkin_trace_info *info, bool child=false);

}
#endif /* end of include guard: ZTRACER_H */
