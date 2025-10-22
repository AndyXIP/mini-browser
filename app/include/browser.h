#ifndef BROWSER_H
#define BROWSER_H

#include "window.h"
#include "searchbar.h"

class Browser {
    private:
        Window window;
        SearchBar searchBar;

    public:
        Browser();
        void run();
};

#endif