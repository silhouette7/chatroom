#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "common.h"

class Client
{
public:
	//初始化socket、epoll参数
	Client();
	//启动socket、epoll
	void Connect(const char *serverip = SERVER_IP, int portnum = PORTNUM);
	//启动客户端
	void Start();
	//关闭客户端
	// ~Client();
private:
	void Close();
	bool working;
	struct sockaddr_in servadd;
	int sockid;
	int epfd;
	int pipefd[2];
	int pid;
};

#endif