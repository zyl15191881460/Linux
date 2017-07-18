#ifndef _UDP_SERVER_
#define _UDP_SERVER_

#include <iostream>
using namespace std;
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
//#include "log.h"


class udp_server
{
	public:
		udp_server(const std::string &_ip, const int &_port);
		int init_server();
		int recv_msg(string &in);
		int send_msg(const string &out, struct sockaddr_in &remote);
		int brocast_msg(); //广播
		
	private:
		~udp_server();
		void addUser(struct sockaddr_in&remote);
		std::string ip;
		int port;
		int sock;
		map<int, struct sockaddr_in> userlist;
};


#endif
