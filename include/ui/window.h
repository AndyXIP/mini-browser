#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include "ui/searchbar.h"
#include "ui/content_view.h"

/**
 * @class Window
 * @brief Application window manager and event dispatcher
 * 
 * Creates and manages the main SFML window, handles window events (resize, close),
 * and coordinates event forwarding and rendering for UI components.
 */
class Window {
    private:
        sf::RenderWindow window;
    
    public:
        /**
         * @brief Construct a new Window with default size (800x600)
         */
        Window();
        
        /**
         * @brief Run the main event loop
         * 
         * Enters the main application loop that:
         * - Polls SFML events (keyboard, mouse, window events)
         * - Forwards events to SearchBar and ContentView
         * - Handles window resize and close events
         * - Clears, draws, and displays each frame
         * 
         * This call blocks until the window is closed.
         * 
         * @param searchBar Reference to the search bar component
         * @param content Reference to the content view component
         */
        void run(SearchBar& searchBar, ContentView& content);
        
        /**
         * @brief Draw all UI components to the window
         * 
         * Clears the window with white background, draws the search bar
         * and content view, then presents the frame buffer.
         * 
         * @param searchBar Reference to the search bar to draw
         * @param content Reference to the content view to draw
         */
        void draw(SearchBar& searchBar, ContentView& content);
};

#endif