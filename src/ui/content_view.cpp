#include "ui/content_view.h"

#include <algorithm>
#include <sstream>

ContentView::ContentView()
    : statusText_(font_), bodyText_(font_) {
    // Load the same font used by the search bar; fallback to a system font if needed
    if (!font_.openFromFile("assets/HelveticaNeue.ttc")) {
        // Try a common macOS font
        font_.openFromFile("/System/Library/Fonts/SFNS.ttf");
    }

    statusText_.setFont(font_);
    statusText_.setCharacterSize(14);
    statusText_.setFillColor(sf::Color(50, 50, 50));
    statusText_.setPosition({viewport_.position.x, viewport_.position.y - 18.f});

    bodyText_.setFont(font_);
    bodyText_.setCharacterSize(14);
    bodyText_.setFillColor(sf::Color::Black);
    bodyText_.setPosition({viewport_.position.x, viewport_.position.y});
}

void ContentView::setViewport(const sf::FloatRect& viewport) {
    viewport_ = viewport;
    statusText_.setPosition({viewport_.position.x, viewport_.position.y - 18.f});
    bodyText_.setPosition({viewport_.position.x, viewport_.position.y});
    rewrap();
}

void ContentView::setStatus(const std::string& statusText) {
    statusText_.setString(statusText);
}

void ContentView::setContent(const std::string& text) {
    raw_ = text;
    rewrap();
}

bool ContentView::handleEvent(const sf::Event& event) {
    if (const auto* e = event.getIf<sf::Event::MouseWheelScrolled>()) {
        // Scroll by lines
        float deltaLines = e->delta; // positive up
        float lineStep = static_cast<float>(bodyText_.getCharacterSize()) * 1.4f;
        scrollY_ -= deltaLines * lineStep; // invert so wheel up scrolls up
        scrollY_ = std::max(0.f, scrollY_);
        return true;
    }
    return false;
}

void ContentView::draw(sf::RenderWindow& window) {
    // Set a view/clipping by drawing into the default view and adjusting text position with scroll
    // Simple clip: we won't implement stencil; just offset Y, content may overflow.
    sf::Vector2f basePos(viewport_.position.x, viewport_.position.y - scrollY_);
    bodyText_.setPosition(basePos);

    window.draw(statusText_);
    window.draw(bodyText_);
}

void ContentView::onResize(const sf::Vector2u& size) {
    // Keep 10px margins; search bar uses ~30px height
    float left = 10.f;
    float top = 50.f;
    float width = std::max(0.f, static_cast<float>(size.x) - 2.f * left);
    float height = std::max(0.f, static_cast<float>(size.y) - top - 10.f);
    setViewport(sf::FloatRect({left, top}, {width, height}));
}

void ContentView::rewrap() {
    // Respect existing newlines from the parser and wrap each line independently
    if (raw_.empty()) { wrapped_.clear(); bodyText_.setString(""); return; }

    // Approximate: character width ~ 0.6 * characterSize; adjust for your font
    const float charW = bodyText_.getCharacterSize() * 0.6f;
    const std::size_t maxCols = static_cast<std::size_t>(std::max(1.f, viewport_.size.x / charW));

    std::ostringstream out;
    std::size_t shown = 0;
    const std::size_t limit = 4000; // preview cap to avoid huge draw cost

    std::istringstream lines(raw_);
    std::string line;
    bool firstLine = true;
    while (std::getline(lines, line)) {
        if (!firstLine) out << '\n';
        firstLine = false;

        std::istringstream words(line);
        std::string word;
        std::size_t col = 0;
        while (words >> word) {
            if (shown + word.size() > limit) { break; }
            if (col == 0) {
                out << word;
                col = word.size();
            } else if (col + 1 + word.size() <= maxCols) {
                out << ' ' << word;
                col += 1 + word.size();
            } else {
                out << '\n' << word;
                col = word.size();
            }
            shown += word.size() + 1;
        }
        if (shown >= limit) break;
    }

    wrapped_ = out.str();
    bodyText_.setString(wrapped_);
}
