#include <stdio.h>
#include <sys/socket.h>
#include <cerrno>
#include <stdexcept>
#include <cstring>
#include <bits/ios_base.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include "session.h"

HttpSession::HttpSession(int fd, HttpHandler* handler, int epoll) : fd(fd),
                                                            clientStatus(WANT_READ),
                                                            handler(handler),
                                                            epollfd(epoll),
                                                            leftData(0),
                                                            sentData(0),
                                                            fileBufferCount(0),
                                                            fileBuffer(""),
                                                            flagEAGAIN(false) {
}

HttpSession::~HttpSession() {
}

void HttpSession::Close() {
    request.clear();
    responce.clear();
    responceHeader.clear();
    filePath.clear();
    file.clear();
    close(fd);
}

bool HttpSession::Read() {
    char buf[ BUFFER_SIZE];
    ssize_t n = recv(fd, buf, sizeof(buf), 0);
    if (n == -1 && errno != EAGAIN) {
        throw std::runtime_error("read: " + std::string(strerror(errno)));
    }
    request.append(buf, (size_t) n);
    try {
        handler->RequestHandle(request, responceHeader, filePath, responce);
        leftData = responceHeader.size();
        sentData = 0;
        clientStatus = WANT_HEADER;
        return true;
    } catch (std::runtime_error) {
        return false;
    }
}

void HttpSession::RecvHeader() {
    try {
        if( write(responceHeader.data(), responceHeader.size()) ) {
            if (!filePath.empty()) {
                file.open(filePath, std::ifstream::in);
                clientStatus = WANT_FILE;
            } else {
                if (!responce.empty()) {
                    leftData = responce.size();
                    sentData = 0;
                clientStatus = WANT_RESPONCE;
                } else {
                    clientStatus = WANT_CLOSE;
                }
            }
        }
    } catch (std::runtime_error &ex) {
        throw std::runtime_error("send: " + std::string(strerror(errno)));
    }
}

void HttpSession::RecvFile() {
    if (flagEAGAIN) {
        while (leftData > 0) {
            try {
                write(fileBuffer,fileBufferCount);
            } catch (std::runtime_error) {
                flagEAGAIN = true;
                return;
            }
        }
        flagEAGAIN = false;
    }
    if (file.is_open()) {
        while ((fileBufferCount = (size_t) file.readsome(fileBuffer, BUFFER_SIZE)) > 0) {
            // mmap
            leftData = fileBufferCount;
            sentData = 0;
            while (leftData > 0) {
                try {
                    write(fileBuffer,fileBufferCount);
                } catch (std::runtime_error) {
                    flagEAGAIN = true;
                    return;
                }
            }
        }
        file.close();
        clientStatus = WANT_CLOSE;
    }
}

bool HttpSession::write(const char *data, size_t size) {
    if (leftData > 0) {
        ssize_t sendResult = send(fd, data + sentData, size - sentData, 0);
        if (sendResult == -1) {
            // sendfile
            throw std::runtime_error("send: " + std::string(strerror(errno)));
        }
        sentData = sendResult;
        leftData -= sentData;
    }
    return leftData == 0;
}

void HttpSession::RecvResponce() {
    try {
        if( write(responce.data(), responce.size()) ) {
            clientStatus = WANT_CLOSE;
        }
    } catch (std::runtime_error &ex) {
        throw std::runtime_error("send: " + std::string(strerror(errno)));
    }
}

ClientStatus HttpSession::Status() {
    return clientStatus;
}
