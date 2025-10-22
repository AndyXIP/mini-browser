#include "searchbar.h"

SearchBar::SearchBar()
  : background({780.f, 30.f}),
    font("assets/HelveticaNeue.ttc"),
    text(font)
{
    background.setFillColor(sf::Color(200, 200, 200));
    background.setPosition({10, 10});

    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setPosition({15, 15});
    text.setString("Hello world");
}

void SearchBar::draw(sf::RenderWindow &window) {
    window.draw(background);
    window.draw(text);
}

void SearchBar::setText(const std::string &str) {
    text.setString(str);
}

std::string SearchBar::getText() const {
    return text.getString();
}