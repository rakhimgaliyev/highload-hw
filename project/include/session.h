#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <fstream>
#include "handler.h"

#define  BUFFER_SIZE 4096

enum ClientStatus{
    WANT_READ, WANT_HEADER, WANT_FILE, WANT_RESPONCE, WANT_CLOSE
};

class HttpSession {
private:
    int fd;
    std::string request;
    std::string responceHeader;
    std::string filePath;
    std::string responce;
    std::ifstream file;
    ClientStatus clientStatus;
    HttpHandler* handler;
    int epollfd;
    ssize_t leftData;
    ssize_t sentData;
    size_t fileBufferCount;
    char fileBuffer[BUFFER_SIZE];
    bool flagEAGAIN;
    bool write (const char *data, size_t size);

public:
    HttpSession(int fd, HttpHandler* handler, int epoll);
    ~HttpSession();

    void Init();
    bool Read();
    void RecvHeader();
    void RecvFile();
    void RecvResponce();
    ClientStatus Status();
    void Close();
};

#endif  // HTTP_SESSION_H
