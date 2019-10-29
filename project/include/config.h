#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdint>
#include <iostream>

#define ADDR "addr"
#define PORT "port"
#define CPU "cpu_limit"
#define ROOT "document_root"
#define QUEUE "queue_size"

#define DEFAULT_ADDR "0.0.0.0"
#define DEFAULT_PORT 80
#define DEFAULT_CPU 1
#define DEFAULT_THREAD 1
#define DEFAULT_ROOT "/"
#define DEFAULT_QUEUE 1024

class Config {
private:
    std::string addr = DEFAULT_ADDR;
    std::uint16_t port = DEFAULT_PORT;
    std::uint16_t cpu = DEFAULT_CPU;
    bool isValid;
    std::string root = DEFAULT_ROOT;
    std::uint32_t queueSize = DEFAULT_QUEUE;

    std::string path;

    void parse();

public:
    Config(std::string path);
    ~Config();

    std::uint16_t Port();
    std::string Addr();
    std::string Root();
    std::uint32_t QueueSize();
    std::uint16_t Cpu();
};

#endif  // CONFIG_H