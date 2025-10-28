#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <vector>

/**
 * @struct Link
 * @brief Represents a clickable link extracted from HTML
 */
struct Link {
    std::string text;
    std::string url;
    size_t start_pos;
    size_t end_pos;
};

/**
 * @struct ParsedPage
 * @brief Complete result of parsing an HTML document
 */
struct ParsedPage {
    std::string title;
    std::string text;
    std::vector<Link> links;
};

/**
 * @brief Parse HTML and extract title, text content, and links
 * 
 * @param html Raw HTML source code as a string
 * @return ParsedPage with title, plain text, and extracted links
 * 
 * @note This is a naive parser, not a full HTML5 spec implementation
 * @note Nested tags and complex structures are handled best-effort
 * @note Malformed HTML may produce unexpected results
 */
ParsedPage parse_html_basic(const std::string& html);

#endif
