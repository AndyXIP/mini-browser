#ifndef CONTENT_VIEW_H
#define CONTENT_VIEW_H

#include <SFML/Graphics.hpp>
#include <string>


class ContentView {
public:
    ContentView();
    void setContent(const std::string& text);
    void setViewport(const sf::FloatRect& viewport);
    bool handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);
    void onResize(const sf::Vector2u& size);
    void setStatus(const std::string& statusText);
private:
    void rewrap();
    sf::Font font_;
    sf::Text statusText_;
    sf::Text bodyText_;
    sf::FloatRect viewport_ { {10.f, 50.f}, {780.f, 540.f} };
    std::string raw_;
    std::string wrapped_;
    float scrollY_ = 0.f;
};

#endif
