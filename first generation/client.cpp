#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>
#include "client.h"

Client::Client() : working(false), pid(-1){}

void Client::Connect(const char *serverip, int portnum)
{
	memset(static_cast<void *>(&servadd), 0, sizeof(servadd));

	servadd.sin_family = AF_INET;

	if(serverip == nullptr){
		struct hostent *hp;
		char hostname[HOSTLEN];
		gethostname(hostname, HOSTLEN);
		hp = gethostbyname(hostname);
		memcpy(static_cast<void *>(&servadd.sin_addr), static_cast<void *>(hp->h_addr), hp->h_length);
	}
	else servadd.sin_addr.s_addr = inet_addr(serverip);

	servadd.sin_port = htons(portnum);

	if((sockid = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		oops("client socket", 2);
	if(connect(sockid, reinterpret_cast<struct sockaddr *>(&servadd), sizeof(servadd)) < 0)
		oops("client connect", 2);
	if(pipe(pipefd) < 0)
		oops("client pipefd", 2);
	if((epfd = epoll_create(EPOLL_SIZE)) < 0)
		oops("client epoll", 2);
	add_event(epfd, sockid);
	add_event(epfd, pipefd[0]);
	working = true;
}

void Client::Start()
{
	pid = fork();
	if(pid < 0)
		oops("client fork", 2);
	//子进程与用户界面交互，接收用户输入的信息
	else if(pid == 0){
		close(pipefd[0]);
		char buf[BUFSIZ];
		while(working){
			fgets(buf, BUFSIZ, stdin);
			//如果用户输入”EXIT“, 则退出
			if(strncmp(buf, EXIT, strlen(EXIT)) == 0)
				working = false;
			//发送用户输入的消息到管道
			else{
				if(write(pipefd[1], buf, strlen(buf)) < 0)
					oops("client write", 2);
			}
		}
	}
	//父进程负责与服务端交互，接收服务端的消息或者发送消息给服务端
	else{
		struct epoll_event epoll_events[EPOLL_SIZE];
		char buf[BUFSIZ];
		close(pipefd[1]);
		while(working){
			memset(buf, 0, BUFSIZ);
			int epoll_count = epoll_wait(epfd, epoll_events, EPOLL_SIZE, -1);
			if(epoll_count < 0)
				oops("client epoll", 2);
			for(int i = 0; i < epoll_count; ++i){
				//从服务端收到消息
				if(epoll_events[i].data.fd == sockid){
					int len = recv(sockid, buf, BUFSIZ, 0);
					if(len == 0){
						//服务端关闭了链接
						std::cout << SERVER_CLOSE << std::endl;
						working = false;
					}
					else{
						//显示服务端发送过来的消息
						std::cout << buf << std::endl;
					}
				}
				//子进程收到用户输入后，通过管道发送消息过来
				else if(epoll_events[i].data.fd == pipefd[0]){
					int len = read(pipefd[0], buf, BUFSIZ);
					//如果用户输入了“EXIT”，则退出
					if(len <= 0)
						working = false;
					//发送消息给服务端
					else{
						if(send(sockid, buf, BUFSIZ, 0) < 0)
							oops("clients send", 2);
					}
				}
			}
		}
	}
	Close();
}

void Client::Close()
{
	if(pid == 0){
		close(pipefd[1]);
	}
	else{
		close(pipefd[0]);
		close(sockid);
		close(epfd);
	}
}