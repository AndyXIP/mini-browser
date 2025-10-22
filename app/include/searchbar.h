#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <SFML/Graphics.hpp>
#include <functional>

class SearchBar {
    private:
        sf::RectangleShape background;
        sf::Text text;
        sf::Font font;
        bool focused = true;
        std::function<void(const std::string&)> onSubmit;
    public:
        SearchBar();
        void draw(sf::RenderWindow &window);
        void setText(const std::string &str);
        std::string getText() const;
        bool handleEvent(const sf::Event& event);
        void setOnSubmit(std::function<void(const std::string&)> callback) { onSubmit = std::move(callback); }
};

#endif