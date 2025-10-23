#ifndef BROWSER_H
#define BROWSER_H

#include "window.h"
#include "searchbar.h"

class Browser {
    private:
        Window window;
        SearchBar searchBar;
        std::string url;
        bool loading = false;
        long status = 0;
        std::string html;
        std::string lastError;

    public:
        Browser();
        void run();
        const std::string& getUrl() const;
        const std::string& getBody() const;
        long getStatus() const;
        bool isLoading() const;
};

#endif