#include <boost/thread.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ztracer.hpp>
#include <iostream>

#define SOCK_PATH "socket"

struct message {
	int  actual_message;
	struct blkin_trace_info trace_info;
	message(int s) : actual_message(s) {};
	message() {}
};

class Parent {
	private:
		int s, s2;
		ZTracer::ZTraceEndpointRef e;	
		ZTracer::ZTraceRef tr;
	public:
		Parent()
		{
			e = ZTracer::create_ZTraceEndpoint("0.0.0.0", 1, "parent");
			connect();
		}
		void operator()() 
		{
			struct sockaddr_un remote;
			int t;
			std::cout << "I am parent process : " << getpid() << std::endl;
			
			/* Wait for connection */
			t = sizeof(remote);
			if ((s2 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&t)) == -1) {
				std::cerr << "accept" << std::endl;
				exit(1);
			}

			std::cerr << "Connected" << std::endl;
			
			for (int i=0;i<10;i++) {
				process();
				
				wait_response();

				/*Log received*/
				tr->event("parent end");
			}	
		}

		void process()
		{
			struct message msg(rand());
			/*Init trace*/
			tr = ZTracer::create_ZTrace("parent process", e);
			/*Annotate*/
			tr->event("parent start");
			/*Set trace info to the message*/
			tr->set_trace_info(&msg.trace_info);	
			
			/*send*/
			send(s2, &msg, sizeof(struct message), 0);
		}
		
		void wait_response() 
		{
			char ack;
			recv(s2, &ack, 1, 0);
		}

		void connect()
		{
			/*create and bind socket*/
			int len;
			struct sockaddr_un local, remote;

			if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
				std::cerr << "socket" << std::endl;
				exit(1);
			}

			local.sun_family = AF_UNIX;
			strcpy(local.sun_path, SOCK_PATH);
			unlink(local.sun_path);
			len = strlen(local.sun_path) + sizeof(local.sun_family);
			if (bind(s, (struct sockaddr *)&local, len) == -1) {
				std::cerr << "bind" << std::endl;
				exit(1);
			}

			if (listen(s, 5) == -1) {
				std::cerr << "listen" << std::endl;
				exit(1);
			}

			std::cout << "Waiting for a connection..." << std::endl;
		}

};

class Child {
	private:
		int s;
		ZTracer::ZTraceEndpointRef e;	
		ZTracer::ZTraceRef tr;
	public:
		Child()
		{
			e = ZTracer::create_ZTraceEndpoint("0.0.0.1", 2, "child");
		}
		void operator()() 
		{
			/*Connect to the socket*/
			soc_connect();	

			for (int i=0;i<10;i++) 
				process();
		}

		void process()
		{
			struct message msg;
			recv(s, &msg, sizeof(struct message), 0);
			
			tr = ZTracer::create_ZTrace("parent process", e, &msg.trace_info);
			tr->event("child start");
			
			usleep(10);
			std::cout << "Message received : " << msg.actual_message << ::std::endl;
			tr->event("child end");
			
			send(s, "*", 1, 0);
		}
		

		void soc_connect()
		{
			int len;
			struct sockaddr_un remote;

			if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
				std::cerr << "socket" << std::endl;
				exit(1);
			}

			std::cout << "Trying to connect...\n" << std::endl;

			remote.sun_family = AF_UNIX;
			strcpy(remote.sun_path, SOCK_PATH);
			len = strlen(remote.sun_path) + sizeof(remote.sun_family);
			if (connect(s, (struct sockaddr *)&remote, len) == -1) {
				std::cerr << "connect" << std::endl;;
				exit(1);
			}

			std::cout << "Connected" << std::endl;
		}

};
int main(int argc, const char *argv[])
{
	Parent p;
	Child c;
	if (fork()) {
		int r = ZTracer::ztrace_init();
		if (r < 0) {
			std::cout << "Error initializing blkin" << std::endl;
			exit(1);
		}
		p();
		exit(0);
	} else {
		int r = ZTracer::ztrace_init();
		if (r < 0) {
			std::cout << "Error initializing blkin" << std::endl;
			exit(1);
		}
		c();
		exit(0);
	}
	return 0;
}
