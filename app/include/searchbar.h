#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <SFML/Graphics.hpp>

class SearchBar {
    private:
        sf::RectangleShape background;
        sf::Text text;
        sf::Font font;
    public:
        SearchBar();
        void draw(sf::RenderWindow &window);
        void setText(const std::string &str);
        std::string getText() const;
};

#endif