#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>

struct HttpResult {
    long status {0};
    std::string body {};
    std::string error {};
};

HttpResult http_get(const std::string& url, int timeout_ms = 10000);

#endif
