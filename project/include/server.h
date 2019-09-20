#ifndef SERVER_H
#define SERVER_H

#include <glob.h>
#include <string>

#include "threadpool.h"
#include "epoll.h"
#include "handler.h"

#define MAX_EPOLL_EVENT 2000
#define EPOLL_TIMEOUT 50

class Server {
private:

    int listenfd;
    bool stop;
    Epoll* epollEngine;
    ThreadPool* threadPool;
    HttpHandler* handler;

public:

    Server(const std::string& addr, const std::uint16_t& port,const std::uint32_t& queueSize,
            const std::string& root, size_t threadCount);
    ~Server();

    void Listen();
};

#endif  // SERVER_H
