#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <sys/epoll.h>
#include <stdlib.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define LISTEN_NUM 10
#define EPOLL_SIZE 5000
#define SERVER_NAME "deric-HP-Spectre-x360-Convertible-13-ae0xx"
#define SERVER_IP "127.0.1.1"
#define WELCOME "|--------------------Welcome to chatroom--------------------|\n"
#define ONECLIENT "There is only you in the chatroom\n"
#define SERVER_CLOSE "Server closed the connection"
#define EXIT "EXIT"


inline void oops(const char* str, int x)
{
	perror(str);
	exit(x);
}

void add_event(int epfd, int fd, int state = EPOLLIN);
void delete_event(int epfd, int fd, int state = EPOLLIN);

#endif