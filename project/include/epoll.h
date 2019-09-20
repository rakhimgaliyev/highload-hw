#ifndef EPOLL_H
#define EPOLL_H

#include <atomic>
#include <sys/epoll.h>

class Epoll {
private:
    int epollfd;
    int maxEpollEvents;
    int timeout;
    std::atomic_bool stop;

public:
    Epoll(int maxEpollEvents, int timeout);
    ~Epoll();

    int AddFd(int clientfd, int epollfd);
    ssize_t Wait(epoll_event* events);
    int Epollfd();
};

#endif  // EPOLL_H