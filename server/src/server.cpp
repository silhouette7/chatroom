#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <iostream>
#include "server.h"

Server::Server(char *hostname, int portnum)
{
	struct hostent *hp;
	memset(static_cast<void *>(&servadd), 0, sizeof(servadd));

	if(hostname == nullptr){
		hostname = new char[HOSTLEN];
		gethostname(hostname, HOSTLEN);
		hp = gethostbyname(hostname);
		delete[] hostname;
	}
	else hp = gethostbyname(hostname);

	memcpy(static_cast<void *>(&servadd.sin_addr), static_cast<void *>(hp->h_addr), hp->h_length);
	servadd.sin_port = htons(portnum);
	servadd.sin_family = AF_INET;
}

//启动socket、epoll
void Server::Init()
{
	if((sockid = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		oops("server socket", 1);
	if(bind(sockid, reinterpret_cast<struct sockaddr *>(&servadd), sizeof(struct sockaddr)) != 0)
		oops("server bind", 1);
	if(listen(sockid, 1) != 0)
		oops("server listen", LISTEN_NUM);
	if((epfd = epoll_create(EPOLL_SIZE)) < 0)
		oops("server epoll", 1);
	add_event(epfd, sockid);

	std::cout << "Init Sucessed" << std::endl;
}

//收到消息时，向聊天室内的其他用户发送消息
int Server::SendBroadcastMessage(int clientfd)
{
	char buf[BUFSIZ];
	int len = recv(clientfd, buf, BUFSIZ, 0);
	if(len < 0)
		return len;
	if(len == 0){
		//客户端关闭了链接，移除相关客户端的记录
		clients.remove(clientfd);
		delete_event(epfd, clientfd);
		//显示客户端离开了聊天室
		std::cout << "client "
		          << clientfd 
		          << "has leave the chatroom"
		          << std::endl;
		std::cout << "Now, there are "
			<< clients.size()
			<< " clients in the chat room"
			<< std::endl;
	}
	else{
		std::cout << buf << std::endl;
		if(clients.size() == 1){
			//如果聊天室内只剩下了发送消息的那个人，就提醒他聊天室内只剩下你了
			//send(clientfd, ONECLIENT, sizeof(ONECLIENT), 0);
		}
		else{
			//向其他客户端发送消息
			std::list<int>::iterator iter;
			for(iter = clients.begin(); iter != clients.end(); ++iter){
				if(*iter == clientfd)
					continue;
				else{
					if(send(*iter, buf, sizeof(buf), 0) < 0)
						return -1;
				}
			}
		}
	}
	return len;
}

//主循环
void Server::Start()
{
	struct epoll_event epoll_events[EPOLL_SIZE];
	Init();
	while(true){
		std::cout << "Enter Loop" << std::endl;
		int epoll_count = epoll_wait(epfd, epoll_events, EPOLL_SIZE, -1);

		if(epoll_count < 0)
			oops("server epoll_wait", 1);

		for(int i = 0; i < epoll_count; ++i){
			if(epoll_events[i].data.fd == sockid){
				//由新客户加入了聊天室
				struct sockaddr_in clientadd;
				socklen_t clientadd_len = sizeof(clientadd);
				int clientfd = accept(sockid, reinterpret_cast<sockaddr *>(&clientadd), &clientadd_len);
				if(clientfd == -1)
					oops("server accept", 1);

				//在记录表中加入新客户
				clients.push_back(clientfd);
				add_event(epfd, clientfd);

				std::cout << "connection from "
				 	 << inet_ntoa(clientadd.sin_addr)
				 	 << ":" << ntohs(clientadd.sin_port)
				 	 << ", clientfd: " << clientfd
				 	 << std::endl;
				std::cout << "Now, there are "
					<< clients.size()
					<< " clients in the chat room"
					<< std::endl;

				//向新客户发送欢迎信息
				if(send(clientfd, WELCOME, sizeof(WELCOME), 0) < 0)
					oops("server send", 1);
			}
			else{
				//如果由客户发送消息，就将该消息广播给其他客户端
				if(SendBroadcastMessage(epoll_events[i].data.fd) < 0)
					oops("SendBroadcastMessage", 1);
			}
		}
	}
}

void Server::Close()
{
	close(sockid);
	close(epfd);
}

Server::~Server()
{
	Close();
}