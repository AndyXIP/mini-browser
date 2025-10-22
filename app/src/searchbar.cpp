#include "searchbar.h"

SearchBar::SearchBar()
  : background({780.f, 30.f}),
    font("assets/HelveticaNeue.ttc"),
    text(font)
{
    background.setFillColor(sf::Color(200, 200, 200));
    background.setPosition({10, 10});

    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setPosition({15, 15});
    text.setString("https://[URL here]");
}

void SearchBar::draw(sf::RenderWindow &window) {
    window.draw(background);
    window.draw(text);
}

void SearchBar::setText(const std::string &str) {
    text.setString(str);
}

std::string SearchBar::getText() const {
    return text.getString();
}

bool SearchBar::handleEvent(const sf::Event& event) {
    if (const auto* e = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (e->button == sf::Mouse::Button::Left) {
            sf::Vector2f pos(static_cast<float>(e->position.x), static_cast<float>(e->position.y));
            focused = background.getGlobalBounds().contains(pos);
            return true;
        }
    }

    if (!focused) return false;

    if (const auto* e = event.getIf<sf::Event::TextEntered>()) {
        char32_t u = e->unicode;
        if (u >= 32 && u != 127) {
            sf::String s = text.getString();
            s += u;
            text.setString(s);
        }
        return true;
    }

    if (const auto* e = event.getIf<sf::Event::KeyPressed>()) {
        using Sc = sf::Keyboard::Scancode;
        if (e->scancode == Sc::Backspace) {
            sf::String s = text.getString();
            if (!s.isEmpty()) {
                s.erase(s.getSize() - 1, 1);
                text.setString(s);
            }
            return true;
        }
        if (e->scancode == Sc::Enter) {
            if (onSubmit) {
                onSubmit(getText());
            }
            return true;
        }
    }

    return false;
}