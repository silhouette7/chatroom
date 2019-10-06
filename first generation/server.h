#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include "common.h"

class Server{
public:
	//初始化socket参数
	Server(char *hostname = nullptr, int portnum = PORTNUM);
	//发送信息
	int SendBroadcastMessage(int clientfd);
	//启动服务器，进入主循环
	void Start();
	~Server();
private:
	//申请socket、epoll
	void Init();
	//关闭服务器
	void Close();
	struct sockaddr_in servadd;
	int sockid;
	int epfd;
	std::list<int> clients;
};

#endif