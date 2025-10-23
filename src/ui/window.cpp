#include "ui/window.h"

Window::Window() : window(sf::VideoMode({800, 600}), "mini browser") {
    window.setFramerateLimit(60);
}

void Window::run(SearchBar &searchBar, ContentView &content) {
    content.onResize(window.getSize());
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::Resized>()) {
                // update the default view to the new size
                auto sz = window.getSize();
                sf::FloatRect rect({0.f, 0.f}, {static_cast<float>(sz.x), static_cast<float>(sz.y)});
                sf::View view(rect);
                window.setView(view);
                content.onResize(sz);
            }

            // Forward events to the search bar for text input and focus
            searchBar.handleEvent(*event);

            // Forward events to content for scrolling
            content.handleEvent(*event);
        }

        draw(searchBar, content);
    }
}

void Window::draw(SearchBar &searchBar, ContentView &content) {
    window.clear(sf::Color::White);
    searchBar.draw(window);
    content.draw(window);
    window.display();
}