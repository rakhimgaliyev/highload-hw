#include <sstream>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <chrono>
#include <sys/stat.h>
#include <fstream>

#include "handler.h"

bool HttpHandler::readReq(std::string &str, std::istringstream &stream) {
    while (stream.peek() != EOF) {
        char c = (char)stream.get();
        if (c == ' ' || c == '\r' || c == '\n') {
            return true;
        } else {
            str.push_back(c);
        }
    }

    return false;
}

std::string HttpHandler::urlDecode(const std::string &url) {
    std::string result;
    for (size_t i = 0; i < url.length(); i++) {
        if (url[i] == '+') {
            result += ' ';
        } else if (url[i] == '%') {
            int ch;
            sscanf(url.substr(i + 1,2).c_str(), "%x", &ch);
            result += static_cast<char>(ch);
            i += 2;
        } else {
            result += url[i];
        }
    }

    return result;
}

bool HttpHandler::fileExists(const std::string &path) {
    return access(path.c_str(), 0) != -1;
}

std::string HttpHandler::getCode(int code) {
    switch (code) {
        case 200:
            return "200 OK";
        case 403:
            return "403 Forbidden";
        case 404:
            return "404 Not Found";
        case 405:
            return "405 Method Not Allowed";
        case 501:
            return "501 Not Implemented";
        default:
            return "";
    }
}

std::string HttpHandler::getFileType(const std::string &path) {
    size_t pos = path.find_last_of('.');
    return (pos != std::string::npos) ? path.substr(pos + 1) : "txt";
}

std::string HttpHandler::buildHeader(size_t fileLength, const std::string &fileType, const std::string &protocol,
        const std::string &code) {
    std::ostringstream response;
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);

    response << protocol
             << ' ' << code << "\r\n"
             << "Server: WebServer \r\n"
             << "Date: " << std::ctime(&t)
             << "Connection: close\r\n"
             << "Content-Length: " << fileLength << "\r\n"
             << "Content-Type: " << mime_map[fileType] << "\r\n";

    return response.str();
}


void HttpHandler::GET(const std::string &url, const std::string &protocol, std::string& header, std::string& path,
                       std::string& responce) {
    std::string pathTemp = rootDir + url;
    int code;
    struct stat s;
    bool dir = ((stat(pathTemp.c_str(), &s) == 0) && (s.st_mode & S_IFDIR));
    if (dir) {
        if (pathTemp.back() != '/') {
            pathTemp += '/';
        }
        pathTemp += "index.html";
    }

    if (fileExists(pathTemp)) {
        code = 200;
        std::string type = dir ? "html" : getFileType(url);
        struct stat file;
        stat(pathTemp.c_str(), &file);
        header = buildHeader((size_t )file.st_size, type, protocol, getCode(code));
        header.append("\r\n");
        path = pathTemp;
    } else {
        code = (dir) ? 403 : 404;
        header  = buildHeader(strlen((dir) ? forbidden : notFound), "html", protocol, getCode(code));
        header.append("\r\n");
        responce = (dir) ? forbidden : notFound;
    }
}

void HttpHandler::HEAD(const std::string &url, const std::string &protocol, std::string& header) {
    std::string path = rootDir + url;
    int code;
    struct stat s;
    bool dir = ((stat(path.c_str(), &s) == 0) && (s.st_mode & S_IFDIR));
    if (dir) {
        if (path.back() != '/') {
            path += '/';
        }
        path += "index.html";
    }

    if (fileExists(path)) {
        code = 200;
        std::string type = dir ? "html" : getFileType(url);
        struct stat file;
        stat(path.c_str(), &file);
        header = buildHeader((size_t )file.st_size, type, protocol, getCode(code));
        header.append("\r\n");
    } else {
        code = (dir) ? 403 : 404;
        header = buildHeader(strlen((dir) ? forbidden : notFound), "html", protocol, getCode(code));
        header.append("\r\n");
    }
}

void HttpHandler::NotAllowed(const std::string &protocol, std::string& header, std::string& responce) {
    const int code = 405;
    header = buildHeader(strlen(notAllowed), "html", protocol, getCode(code));
    header.append("\r\n");
    responce = notAllowed;
}

void HttpHandler::NotImplemented(const std::string &protocol, std::string& header, std::string& responce) {
    const int code = 501;
    header = buildHeader(strlen(notImplemented), "html", protocol, getCode(code));
    header.append("\r\n");
    responce = notImplemented;
}

void HttpHandler::RequestHandle(std::string request, std::string& header, std::string& path, std::string& responce) {
    std::string method, url, protocol = "HTTP/1.1";
    std::istringstream istringstream(request);

    if(!readReq(method, istringstream)) {
        throw std::runtime_error("bad request");
    }

    if(!readReq(url, istringstream)) {
        throw std::runtime_error("bad request");
    }

    if(!readReq(protocol, istringstream)) {
        throw std::runtime_error("bad request");
    }

    url = urlDecode(url);
    size_t  pos;

    while ((pos = url.find("/../")) != std::string::npos) {
        url.erase(pos, 4);
    }

    if ((pos = url.find('?')) != std::string::npos) {
        url = url.substr(0, pos);
    }

    if (method == "GET") {
        GET(url, protocol, header, path, responce);
    } else if (method == "HEAD") {
        HEAD(url, protocol, header);
    } else if (method == "POST" || method == "PUT" || method == "PATCH" ||
            method == "DELETE" || method == "TRACE" || method == "CONNECT" ||
                method == "OPTIONS") {
        NotAllowed(protocol, header, responce);
    } else {
        NotImplemented(protocol, header, responce);
    }
}

HttpHandler::HttpHandler(const std::string &root) : rootDir(root) {}
