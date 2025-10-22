#include "window.h"

Window::Window() : window(sf::VideoMode({800, 600}), "mini browser") {
    window.setFramerateLimit(60);
}

void Window::run(SearchBar &searchBar) {
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            // Forward events to the search bar for text input and focus
            searchBar.handleEvent(*event);
        }

        draw(searchBar);
    }
}

void Window::draw(SearchBar &searchBar) {
    window.clear(sf::Color::White);
    searchBar.draw(window);
    window.display();
}