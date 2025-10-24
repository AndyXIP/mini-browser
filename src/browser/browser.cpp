#include "browser/browser.h"
#include "core/http_client.h"
#include "core/html_parser.h"
#include <future>
#include <iostream>

Browser::Browser() {
    // When user presses Enter in the search bar, store the URL text
    searchBar.setOnSubmit([this](const std::string& s){
        url = s;
        // Normalize: prepend https:// if missing scheme
        std::string normalized = url;
        if (normalized.rfind("http://", 0) != 0 && normalized.rfind("https://", 0) != 0) {
            normalized = "https://" + normalized;
        }
        // Launch async fetch using std::async and poll in-place via future
        loading = true;
        auto fut = std::async(std::launch::async, [normalized]{
            return http_get(normalized, 10000);
        });
        // Busy-ish polling just once; result will be consumed below (we could store the future if we wanted)
        HttpResult r = fut.get();
        loading = false;
        if (!r.error.empty()) {
            lastError = r.error;
            status = 0;
            html.clear();
        } else {
            lastError.clear();
            status = r.status;
            html = std::move(r.body);
            std::cout << "Fetched status " << status << ", body size: " << html.size() << " bytes\n";
        }
        // Update the UI content view
        if (!lastError.empty()) {
            content.setStatus("Error: " + lastError);
            content.setContent("", {});
        } else {
            auto parsed = parse_html_basic(html);
            std::string statusLine = "HTTP " + std::to_string(status);
            if (!parsed.title.empty()) statusLine += " — " + parsed.title;
            content.setStatus(statusLine);
            content.setContent(parsed.text, parsed.links);
        }
        });

        // Link click navigation
        content.setOnLinkClick([this](const std::string& linkUrl){
            std::string fullUrl = linkUrl;
            // If relative, try to resolve naively
            if (!linkUrl.empty() && linkUrl[0] == '/') {
                auto proto = url.find("://");
                if (proto != std::string::npos) {
                    auto domainEnd = url.find('/', proto + 3);
                    std::string origin = domainEnd == std::string::npos ? url : url.substr(0, domainEnd);
                    fullUrl = origin + linkUrl;
                }
            } else if (linkUrl.find("://") == std::string::npos) {
                fullUrl = "https://" + linkUrl;
            }

            url = fullUrl;
            loading = true;
            auto fut = std::async(std::launch::async, [fullUrl]{ return http_get(fullUrl, 10000); });
            HttpResult r = fut.get();
            loading = false;

            if (!r.error.empty()) {
                lastError = r.error;
                status = 0;
                html.clear();
            } else {
                lastError.clear();
                status = r.status;
                html = std::move(r.body);
            }

            if (!lastError.empty()) {
                content.setStatus("Error: " + lastError);
                content.setContent("", {});
            } else {
                auto parsed = parse_html_basic(html);
                std::string statusLine = "HTTP " + std::to_string(status);
                if (!parsed.title.empty()) statusLine += " — " + parsed.title;
                content.setStatus(statusLine);
                content.setContent(parsed.text, parsed.links);
            }
        });
}

void Browser::run() {
    window.run(searchBar, content);
};

const std::string& Browser::getUrl() const {
    return url;
}

const std::string& Browser::getBody() const {
    return html;
}

long Browser::getStatus() const {
    return status;
}

bool Browser::isLoading() const {
    return loading;
}