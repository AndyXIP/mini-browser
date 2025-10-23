#include "browser.h"
#include "http_client.h"
#include "html_parser.h"
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
            std::cout << "HTTP error: " << lastError << "\n";
        } else {
            lastError.clear();
            status = r.status;
            html = std::move(r.body);
            std::cout << "Fetched status " << status << ", body size: " << html.size() << " bytes\n";
        }
        // Update the UI content view
        if (!lastError.empty()) {
            content.setStatus("Error: " + lastError);
            content.setContent("");
        } else {
            auto parsed = parse_html_basic(html);
            std::string statusLine = "HTTP " + std::to_string(status);
            if (!parsed.title.empty()) statusLine += " — " + parsed.title;
            content.setStatus(statusLine);
            content.setContent(parsed.text);
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