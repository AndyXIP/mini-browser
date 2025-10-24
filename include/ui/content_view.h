#ifndef CONTENT_VIEW_H
#define CONTENT_VIEW_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include "core/html_parser.h"

// Rendered link with screen bounds
struct RenderedLink {
    std::string url;
    sf::FloatRect bounds; // Screen-space bounding box
};

class ContentView {
public:
    ContentView();
    void setContent(const std::string& text, const std::vector<Link>& links = {});
    void setViewport(const sf::FloatRect& viewport);
    bool handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);
    void onResize(const sf::Vector2u& size);
    void setStatus(const std::string& statusText);
    
    // Set callback invoked when a link is clicked
    void setOnLinkClick(std::function<void(const std::string&)> callback) {
        onLinkClick_ = std::move(callback);
    }
    
private:
    void rewrap();
    
    sf::Font font_;
    sf::Text statusText_;
    sf::Text bodyText_;
    sf::FloatRect viewport_ { {10.f, 50.f}, {780.f, 540.f} };
    std::string raw_;
    std::string wrapped_;
    float scrollY_ = 0.f;
    
    std::vector<Link> links_;
    std::vector<RenderedLink> renderedLinks_;
    std::function<void(const std::string&)> onLinkClick_;
};

#endif
