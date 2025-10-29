#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <SFML/Graphics.hpp>
#include <functional>

/**
 * @class SearchBar
 * @brief Interactive URL address bar with text input
 * 
 * The search bar notifies listeners via a callback when Enter is pressed.
 */
class SearchBar {
    private:
        sf::RectangleShape background;
        sf::Text text;
        sf::Font font;
        bool focused = true;
        std::function<void(const std::string&)> onSubmit;

    public:
        /**
         * @brief Construct a new SearchBar
         * 
         * @note Requires assets/HelveticaNeue.ttc font file to be present
         */
        SearchBar();
        
        /**
         * @brief Draw the search bar to the window
         * 
         * @param window Target SFML render window
         */
        void draw(sf::RenderWindow &window);
        
        /**
         * @brief Set the text content programmatically
         * 
         * @param str New text to display in the search bar
         */
        void setText(const std::string &str);
        
        /**
         * @brief Get the current text content
         * 
         * @return std::string Current text in the search bar
         */
        std::string getText() const;
        
        /**
         * @brief Handle SFML input events
         * 
         * Processes:
         * - TextEntered: Appends typed characters to the text
         * - KeyPressed (Backspace): Removes last character
         * - KeyPressed (Enter): Invokes onSubmit callback with current text
         * - MouseButtonPressed: Updates focus state based on click position
         * 
         * @param event SFML event to process
         * @return true if event was handled by this component
         */
        bool handleEvent(const sf::Event& event);
        
        /**
         * @brief Register callback for URL submission
         * 
         * @param callback Function to invoke on submission, signature: void(const std::string&)
         */
        void setOnSubmit(std::function<void(const std::string&)> callback) { 
            onSubmit = std::move(callback); 
        }
};

#endif