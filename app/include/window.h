#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include "searchbar.h"

class Window {
    private:
        sf::RenderWindow window;
    public:
        Window();
        void run(SearchBar& searchBar);
        void draw(SearchBar& searchBar);
};

#endif