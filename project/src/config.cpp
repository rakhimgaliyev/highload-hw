#include <fstream>
#include "config.h"

void Config::parse() {
    std::ifstream conf;
    conf.open(path);
    std::string temp;

    while (!conf.eof()) {
        conf >> temp;
        if (temp == ADDR) {
            conf >> addr;
        }
        if (temp == ROOT) {
            conf >> root;
        }
        if (temp == PORT) {
            conf >> port;
        }
        if (temp == CPU) {
            conf >> cpu;
        }
        if (temp == THREAD) {
            conf >> thread;
        }
        if (temp == QUEUE) {
            conf >> queueSize;
        }
    }
    isValid = true;
    conf.close();
}

Config::Config(std::string path) : path(path + "/httpd.conf") {
    parse();
}

Config::~Config() {}

std::uint16_t Config::Port() {
    return port;
}

std::uint16_t Config::Thread() {
    return thread;
}

std::string Config::Addr() {
    return addr;
}

bool Config::IsValid() {
    return isValid;
}

std::string Config::Root() {
    return root;
}

std::uint32_t Config::QueueSize() {
    return queueSize;
}

std::uint16_t Config::Cpu() {
    return cpu;
}