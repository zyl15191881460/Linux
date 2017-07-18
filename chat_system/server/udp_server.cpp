#include "udp_server.h"



udp_server::udp_server(const std::string &_ip, const int &_port)
	: port(_port)
	, ip(_ip)
{}
int udp_server::init_server()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
//		print_log()
		return -1;
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip.c_str());

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
//		print_log()
		return -2;
	}
	return sock;
}
int udp_server::recv_msg(std::string &in)
{
	struct sockaddr_in remote;
	socklen_t len = sizeof(remote);
	char buf[1024];
	ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&remote, &len);
	if(s > 0)
	{
		buf[s] = 0;
		in = buf;
		put_data(in);//放到data_pool中
		addUser(remote);//把数据记录起来
	}
	return s;
}
int udp_server::send_msg(const std::string &out, sockaddr_in &remote )
{
	ssize_t s = sendto(sock, out.c_str(), out.size(), 0, (sockaddr_in*)&remote, sizeof(remote));
	if(s < 0)
	{
		//print_log();
	}
	return s;
}
int udp_server::brocast_msg() //广播
{
	string out;
	getData(out);
	auto it = userlist.begin();
	for(; it != userlist.end(); ++it)
	{
		send_msg(out, it->second);
	}
}
int udp_server::addUser(struct sockadddr_in& remote)
{
	userlist.insert(std::pair<int, sockaddr_in>(remote.sin_addr.s_addr, remote));
}
udp_server::~udp_server()
{
	if(sock > 0)
		close(sock);
}


