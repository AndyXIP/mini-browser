#include "browser.h"
#include "http_client.h"
#include <thread>
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
        // Launch background fetch so the render loop is not blocked
        loading = true;
        std::thread([this, normalized]{
            HttpResult r = http_get(normalized, 10000);
            // Update state (no synchronization here; simple demo)
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
        }).detach();
    });
}

void Browser::run() {
    window.run(searchBar);
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