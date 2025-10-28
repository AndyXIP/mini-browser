#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>

/**
 * @struct HttpResult
 * @brief Result of an HTTP GET request
 */
struct HttpResult {
    long status {0};
    std::string body {};
    std::string error {};
};

/**
 * @brief Perform a blocking HTTP GET request
 * 
 * @param url The full URL to fetch (must include http:// or https://)
 * @param timeout_ms Request timeout in milliseconds (default: 10 seconds)
 * @return HttpResult containing status, body, and error information
 * 
 * @note This call blocks until the request completes or times out
 * @note Requires libcurl to be installed and linked
 */
HttpResult http_get(const std::string& url, int timeout_ms = 10000);

#endif
