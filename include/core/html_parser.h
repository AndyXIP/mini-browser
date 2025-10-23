#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>

struct ParsedPage {
    std::string title;
    std::string text;
};

ParsedPage parse_html_basic(const std::string& html);

#endif
