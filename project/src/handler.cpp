#include <sstream>
#include <unistd.h>

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

std::string HttpHandler::getCode(int &code) {
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
    return (pos != -1) ? path.substr(pos + 1) : "txt";
}