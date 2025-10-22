#include "browser.h"

Browser::Browser() {
    // When user presses Enter in the search bar, store the URL text
    searchBar.setOnSubmit([this](const std::string& s){
        url = s;
    });
}

void Browser::run() {
    window.run(searchBar);
};

const std::string& Browser::getUrl() const {
    return url;
}