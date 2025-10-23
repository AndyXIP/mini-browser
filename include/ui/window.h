#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include "ui/searchbar.h"
#include "ui/content_view.h"

class Window {
    private:
        sf::RenderWindow window;
    public:
        Window();
        void run(SearchBar& searchBar, ContentView& content);
        void draw(SearchBar& searchBar, ContentView& content);
};

#endif