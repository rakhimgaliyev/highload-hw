#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <string>
#include <sstream>
#include <unordered_map>

class HttpHandler {
private:
    std::string rootDir;

    std::unordered_map <std::string, std::string> mine_map =
    {
        {"txt", "text/plain"},
        {"html", "text/html"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"png", "image/png"},
        {"gif", "image/gif"},
        {"swf", "application/x-shockwave-flash"}
    };
    
    const std::string forbidden =
    "<html>"
        "<head><title>Forbidden</title></head>"
        "<body><h1>403 Forbidden</h1></body>"
    "</html>";
    
    const std::string notFound =
    "<html>"
        "<head><title>Not Found</title></head>"
        "<body><h1>404 Not Found</h1></body>"
    "</html>";

    const std::string notAllowed =
    "<html>"
        "<head><title>Not Allowed</title></head>"
        "<body><h1>405 Method Not Allowed</h1></body>"
    "</html>";

    const std::string notImplemented =
    "<html>"
        "<head><title>Not Implemented</title></head>"
        "<body><h1>501 Not Implemented</h1></body>"
    "</html>";

    bool readReq(std::string &str, std::istringstream &stream);
    bool fileExists(const std::string& path);

    std::string urlDecode(const std::string &url);
    std::string getCode(int &code);
    std::string getFileType(const std::string& path);
    std::string buidHeader(size_t &fileLength, const std::string& fileType,
        const std::string &protocol,  const std::string &code);
    
    void GET(const std::string& url, const std::string& protocol, std::string& header, std::string& path,
        std::string& responce);
    void HEAD(const std::string& url, const std::string& protocol, std::string& header, std::string& responce);
    void NotAllowed(const std::string& protocol, std::string& header, std::string& responce);
    void NotImplemented(const std::string& protocol, std::string& header, std::string& responce);




};

#endif  // HTTP_HANDLER_H
