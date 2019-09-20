#include <unistd.h>
#include "epoll.h"

Epoll::Epoll(int maxEpollEvents, int timeout) :
        maxEpollEvents(maxEpollEvents), timeout(timeout), stop(false) {
    epollfd = epoll_create(maxEpollEvents);
}

int Epoll::AddFd(int clientfd, int epollfd) {
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLOUT ;
    ev.data.fd = clientfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
    return clientfd;
}

ssize_t Epoll::Wait(epoll_event* events) {
    return epoll_wait(epollfd, events, maxEpollEvents, timeout);
}


int Epoll::Epollfd() {
    return epollfd;
}

Epoll::~Epoll() {}
