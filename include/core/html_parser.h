#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <vector>

struct Link {
    std::string text;  // Link display text
    std::string url;   // href attribute
    size_t start_pos;  // Character position in the plain text where link starts
    size_t end_pos;    // Character position where link ends
};

struct ParsedPage {
    std::string title;
    std::string text;
    std::vector<Link> links;
};

ParsedPage parse_html_basic(const std::string& html);

#endif
