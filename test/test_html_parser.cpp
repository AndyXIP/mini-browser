#include "test.h"
#include "core/html_parser.h"
#include <string>

TEST(test_parse_title) {
    std::string html = "<html><head><title>Test Page</title></head><body>Content</body></html>";
    ParsedPage page = parse_html_basic(html);
    ASSERT_EQ(std::string("Test Page"), page.title, "Title should be extracted");
}

TEST(test_strip_tags) {
    std::string html = "<p>Hello <strong>World</strong></p>";
    ParsedPage page = parse_html_basic(html);
    ASSERT(page.text.find("Hello World") != std::string::npos, "Tags should be stripped");
}

TEST(test_decode_entities) {
    std::string html = "<p>&lt;tag&gt; &amp; &quot;quotes&quot;</p>";
    ParsedPage page = parse_html_basic(html);
    ASSERT(page.text.find("<tag>") != std::string::npos, "Should decode &lt; and &gt;");
    ASSERT(page.text.find("&") != std::string::npos, "Should decode &amp;");
    ASSERT(page.text.find("\"quotes\"") != std::string::npos, "Should decode &quot;");
}

TEST(test_strip_script_style) {
    std::string html = "<html><head><script>alert('hi');</script></head><body>Text<style>css{}</style></body></html>";
    ParsedPage page = parse_html_basic(html);
    ASSERT(page.text.find("alert") == std::string::npos, "Script content should be removed");
    ASSERT(page.text.find("css") == std::string::npos, "Style content should be removed");
    ASSERT(page.text.find("Text") != std::string::npos, "Body text should remain");
}

TEST(test_extract_links) {
    std::string html = R"(<html><body><a href="https://example.com">Example Link</a> and <a href="/page">Relative</a></body></html>)";
    ParsedPage page = parse_html_basic(html);
    
    ASSERT_EQ(2u, page.links.size(), "Should extract 2 links");
    
    if (page.links.size() >= 1) {
        ASSERT_EQ(std::string("Example Link"), page.links[0].text, "First link text");
        ASSERT_EQ(std::string("https://example.com"), page.links[0].url, "First link URL");
    }
    
    if (page.links.size() >= 2) {
        ASSERT_EQ(std::string("Relative"), page.links[1].text, "Second link text");
        ASSERT_EQ(std::string("/page"), page.links[1].url, "Second link URL");
    }
}

TEST(test_preserve_newlines) {
    std::string html = "<p>First paragraph</p><br/><p>Second paragraph</p>";
    ParsedPage page = parse_html_basic(html);
    ASSERT(page.text.find("First paragraph") != std::string::npos, "First paragraph text");
    ASSERT(page.text.find("Second paragraph") != std::string::npos, "Second paragraph text");
    // Newlines should be present between paragraphs
    ASSERT(page.text.find('\n') != std::string::npos, "Should contain newlines");
}

TEST(test_empty_html) {
    std::string html = "";
    ParsedPage page = parse_html_basic(html);
    ASSERT(page.title.empty(), "Empty HTML should have empty title");
    ASSERT(page.text.empty(), "Empty HTML should have empty text");
    ASSERT(page.links.empty(), "Empty HTML should have no links");
}
