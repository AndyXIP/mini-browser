#include "html_parser.h"

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

    result.text = std::move(body);
    return result;
}
