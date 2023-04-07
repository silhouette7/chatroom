#include "common.h"

void add_event(int epfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) < 0)
		oops("epoll_ctl_add", 3);
}

void delete_event(int epfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	if(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev) < 0)
		oops("epoll_ctl_del", 3);
}