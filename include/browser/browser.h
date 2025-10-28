#ifndef BROWSER_H
#define BROWSER_H

#include "ui/window.h"
#include "ui/searchbar.h"
#include "ui/content_view.h"

/**
 * @class Browser
 * @brief Top-level browser application controller
 * 
 * This is the entry point for the application after main() creates an instance
 * and calls run().
 */
class Browser {
    private:
        Window window;
        SearchBar searchBar;
        ContentView content;
        std::string url;
        bool loading = false;
        long status = 0;
        std::string html;
        std::string lastError;

    public:
        /**
         * @brief Construct a new Browser instance
         */
        Browser();
        
        /**
         * @brief Run the browser application
         * 
         * @note Call this method only once per Browser instance
         */
        void run();
        
        /**
         * @brief Get the current URL
         * 
         * @return const std::string& Reference to the currently loaded URL
         */
        const std::string& getUrl() const;
        
        /**
         * @brief Get the raw HTML body of the current page
         * 
         * @return const std::string& Reference to the HTML source
         */
        const std::string& getBody() const;
        
        /**
         * @brief Get the HTTP status code of the last request
         * 
         * @return long HTTP status code (200, 404, 500, etc.), 0 if not yet loaded
         */
        long getStatus() const;
        
        /**
         * @brief Check if a page is currently loading
         * 
         * @return true if an HTTP request is in progress
         * @return false if idle or request completed
         */
        bool isLoading() const;
};

#endif