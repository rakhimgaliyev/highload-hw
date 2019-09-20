#include <iostream>

#include "server.h"
#include "config.h"

int main(int argc, char* argv[]) {
    std::string rootDir = argv[0];
    rootDir = rootDir.substr(0, rootDir.find_last_of('/'));
    Config* config = new Config("/etc");

    Server* server = new Server(config->Addr(), config->Port(), config->QueueSize(), config->Root(), config->Cpu());

    server->Listen();
    std::cout << "Listening" << std::endl;

    delete server;
    delete config;

    return 0;
}