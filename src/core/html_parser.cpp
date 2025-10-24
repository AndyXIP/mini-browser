#include "core/html_parser.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>

namespace {

static std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    return s;
}

static void erase_range(std::string& s, std::size_t pos, std::size_t len) {
    if (pos < s.size()) s.erase(pos, std::min(len, s.size() - pos));
}

static void strip_blocks(std::string& s, const std::string& openTag, const std::string& closeTag) {
    // Remove blocks like <script ...> ... </script> (case-insensitive)
    std::string sl = to_lower(s);
    std::string o = to_lower(openTag);
    std::string c = to_lower(closeTag);
    std::size_t p = 0;
    while (true) {
        p = sl.find(o, p);
        if (p == std::string::npos) break;
        std::size_t end = sl.find(c, p + o.size());
        if (end == std::string::npos) {
            // remove rest
            erase_range(s, p, s.size() - p);
            break;
        } else {
            // include close tag length
            std::size_t len = (end - p) + c.size();
            erase_range(s, p, len);
            sl = to_lower(s);
        }
    }
}

static std::string strip_tags(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    bool in_tag = false;
    for (char ch : s) {
        if (ch == '<') { in_tag = true; continue; }
        if (ch == '>') { in_tag = false; continue; }
        if (!in_tag) out.push_back(ch);
    }
    return out;
}

// Replace occurrences of specific HTML tags with newlines (case-insensitive)
static void replace_all_ci(std::string& s, const std::string& needle, const std::string& repl) {
    if (needle.empty()) return;
    std::string lower = to_lower(s);
    std::string nlow = to_lower(needle);
    std::size_t pos = 0;
    while ((pos = lower.find(nlow, pos)) != std::string::npos) {
        s.replace(pos, needle.size(), repl);
        lower.replace(pos, needle.size(), repl);
        pos += repl.size();
    }
}

static void normalize_linebreak_tags(std::string& s) {
    // line-break-like tags to map to '\n'
    const char* brTags[] = {
        "<br>", "<br/>", "<br />",
        "<hr>", "<hr/>", "<hr />",
        "</p>", "<p>",
        "</div>", "</section>", "</article>", "</header>", "</footer>",
        "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>",
        "</li>", "</ul>", "</ol>"
    };
    for (const char* t : brTags) {
        replace_all_ci(s, t, "\n");
    }
}

static std::string decode_entities(const std::string& s) {
    std::ostringstream out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '&') {
            if (s.compare(i, 5, "&amp;") == 0) { out << '&'; i += 4; }
            else if (s.compare(i, 4, "&lt;") == 0) { out << '<'; i += 3; }
            else if (s.compare(i, 4, "&gt;") == 0) { out << '>'; i += 3; }
            else if (s.compare(i, 6, "&quot;") == 0) { out << '"'; i += 5; }
            else if (s.compare(i, 6, "&apos;") == 0) { out << '\''; i += 5; }
            else {
                out << '&';
            }
        } else {
            out << s[i];
        }
    }
    return out.str();
}

static std::string trim_lines(const std::string& s) {
    std::istringstream in(s);
    std::ostringstream out;
    std::string line;
    bool first = true;
    while (std::getline(in, line)) {
        // trim spaces
        auto begin = line.find_first_not_of(" \t\r");
        auto end = line.find_last_not_of(" \t\r");
        if (begin == std::string::npos) continue;
        line = line.substr(begin, end - begin + 1);
        if (!first) out << '\n';
        out << line;
        first = false;
    }
    return out.str();
}

// Extract links from HTML: find <a href="...">text</a> and record positions
static std::vector<Link> extract_links(const std::string& html, const std::string& plainText) {
    std::vector<Link> links;
    std::string lower = to_lower(html);
    
    size_t pos = 0;
    while (true) {
        // Find next <a tag
        pos = lower.find("<a ", pos);
        if (pos == std::string::npos) break;
        
        // Find href attribute
        size_t href_start = lower.find("href=", pos);
        if (href_start == std::string::npos || href_start > lower.find('>', pos)) {
            pos++;
            continue;
        }
        
        href_start += 5; // skip "href="
        char quote = html[href_start];
        size_t href_end;
        if (quote == '"' || quote == '\'') {
            href_start++;
            href_end = html.find(quote, href_start);
        } else {
            href_end = html.find_first_of(" >", href_start);
        }
        
        if (href_end == std::string::npos) { pos++; continue; }
        std::string url = html.substr(href_start, href_end - href_start);
        
        // Find closing >
        size_t tag_close = html.find('>', href_end);
        if (tag_close == std::string::npos) { pos++; continue; }
        
        // Find </a>
        size_t end_tag = lower.find("</a>", tag_close);
        if (end_tag == std::string::npos) { pos++; continue; }
        
        // Extract link text and strip inner tags
        std::string link_text = html.substr(tag_close + 1, end_tag - (tag_close + 1));
        link_text = strip_tags(link_text);
        link_text = decode_entities(link_text);
        link_text = trim_lines(link_text);
        
        // Find position in plain text (approximate: search for link text)
        size_t text_pos = plainText.find(link_text);
        if (text_pos != std::string::npos && !link_text.empty()) {
            Link link;
            link.text = link_text;
            link.url = url;
            link.start_pos = text_pos;
            link.end_pos = text_pos + link_text.size();
            links.push_back(link);
        }
        
        pos = end_tag + 4;
    }
    
    return links;
}

}

ParsedPage parse_html_basic(const std::string& html) {
    ParsedPage result;

    // Find <title> (case-insensitive)
    std::string lower = to_lower(html);
    auto t1 = lower.find("<title");
    if (t1 != std::string::npos) {
        auto t1_end = lower.find('>', t1);
        auto t2 = lower.find("</title>", t1_end == std::string::npos ? t1 : t1_end);
        if (t1_end != std::string::npos && t2 != std::string::npos && t2 > t1_end) {
            std::string title = html.substr(t1_end + 1, t2 - (t1_end + 1));
            result.title = trim_lines(decode_entities(strip_tags(title)));
        }
    }

    // Create a copy, normalize line-break-like tags, remove script/style blocks, then strip tags
    std::string body = html;
    normalize_linebreak_tags(body);
    strip_blocks(body, "<script", "</script>");
    strip_blocks(body, "<style", "</style>");
    body = strip_tags(body);
    body = decode_entities(body);
    body = trim_lines(body);

    result.text = body;
    
    // Extract links and their positions
    result.links = extract_links(html, result.text);
    
    return result;
}
