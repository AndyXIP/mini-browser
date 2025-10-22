#ifndef BROWSER_H
#define BROWSER_H

#include "window.h"
#include "searchbar.h"

class Browser {
    private:
        Window window;
        SearchBar searchBar;
        std::string url;

    public:
        Browser();
        void run();
        const std::string& getUrl() const;
};

#endif