#ifndef CONTENT_VIEW_H
#define CONTENT_VIEW_H

#include <SFML/Graphics.hpp>
#include <string>

// Very simple text viewer with naive word wrapping and vertical scrolling.
class ContentView {
public:
    ContentView();

    // Set the raw text to display (will rewrap)
    void setContent(const std::string& text);

    // Set the available drawing area (position and size) in window coords
    void setViewport(const sf::FloatRect& viewport); // x,y,width,height

    // Handle input (mouse wheel to scroll)
    bool handleEvent(const sf::Event& event);

    // Draw into window
    void draw(sf::RenderWindow& window);

    // Update layout on window resize
    void onResize(const sf::Vector2u& size);

    // Optional: display a status line (e.g., Loading..., HTTP 200)
    void setStatus(const std::string& statusText);

private:
    void rewrap();

    sf::Font font_;
    sf::Text statusText_;
    sf::Text bodyText_;

    sf::FloatRect viewport_ { {10.f, 50.f}, {780.f, 540.f} };
    std::string raw_;
    std::string wrapped_;

    float scrollY_ = 0.f; // positive scrolls down
};

#endif // CONTENT_VIEW_H
