#ifndef CONTENT_VIEW_H
#define CONTENT_VIEW_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include "core/html_parser.h"

/**
 * @struct RenderedLink
 * @brief Rendered link with screen-space bounds for click detection
 */
struct RenderedLink {
    std::string url;
    sf::FloatRect bounds;
};

/**
 * @class ContentView
 * @brief Scrollable text viewer with clickable links
 */
class ContentView {
public:
    /**
     * @brief Construct a new ContentView
     * 
     * Initializes the content viewer with default viewport, loads the font,
     * and sets up text rendering. The view starts with empty content.
     * 
     * @note Requires assets/HelveticaNeue.ttc font file to be present
     */
    ContentView();
    
    /**
     * @brief Set content to display with optional links
     * 
     * @param text Plain text content to display (can include newlines)
     * @param links Vector of links with positions matching text indices
     * 
     * @note Link positions (start_pos, end_pos) must correspond to character
     *       indices in the text parameter for correct rendering
     */
    void setContent(const std::string& text, const std::vector<Link>& links = {});
    
    /**
     * @brief Set the viewport rectangle for content rendering
     * 
     * @param viewport Rectangle defining content area (position and size)
     */
    void setViewport(const sf::FloatRect& viewport);
    
    /**
     * @brief Handle SFML input events
     * 
     * Processes:
     * - MouseWheelScrolled: Scrolls content up/down
     * - MouseButtonPressed: Detects clicks on links and invokes onLinkClick callback
     * 
     * @param event SFML event to process
     * @return true if event was handled by this component
     */
    bool handleEvent(const sf::Event& event);
    
    /**
     * @brief Draw content to the window
     * 
     * Renders the status text, body text with word wrapping, and draws
     * blue underlines beneath clickable links. Applies scroll offset and
     * viewport clipping.
     * 
     * @param window Target SFML render window
     */
    void draw(sf::RenderWindow& window);
    
    /**
     * @brief Handle window resize events
     * 
     * Updates the viewport dimensions and triggers re-wrapping of text
     * to fit the new width. Call this when the window is resized.
     * 
     * @param size New window size in pixels
     */
    void onResize(const sf::Vector2u& size);
    
    /**
     * @brief Set status text displayed at top of content area
     * 
     * @param statusText Status message to display (empty to hide)
     */
    void setStatus(const std::string& statusText);
    
    /**
     * @brief Register callback for link clicks
     * 
     * Sets a function to be called when the user clicks on a rendered link.
     * The callback receives the target URL as a parameter.
     * 
     * @param callback Function to invoke on link click, signature: void(const std::string&)
     */
    void setOnLinkClick(std::function<void(const std::string&)> callback) {
        onLinkClick_ = std::move(callback);
    }
    
private:
    /**
     * @brief Perform word wrapping on content text
     * 
     * Splits text into lines that fit within the viewport width, respecting
     * existing newlines from HTML formatting. Updates the wrapped_ string
     * used for rendering.
     */
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
