#ifndef CONFIG_H
#define CONFIG_H

#include <string>

const std::string DEFAULT_ADDR = "0.0.0.0";
const std::string DEFAULT_DOCUMENT_ROOT = "/var/www/html";
const int CPU_LIMIT    = 4;
const int DEFAULT_PORT = 80;
const int THREAD_LIMIT = 256;

#endif  // CONFIG_H