#include "button.h"
#include <iostream>

Button::Button(int x, int y, int width, int height, string text, guiStyle baseStyle, guiStyle hoverStyle) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->rectangle = new sf::RectangleShape(sf::Vector2f(width, height));
    this->rectangle->setSize(sf::Vector2f(width, height));
    this->rectangle->setPosition(x, y);

    this->baseStyle = baseStyle;
    this->hoverStyle = hoverStyle;
    this->text = text;
}

void Button::applyBaseStyle() {

    this->rectangle->setFillColor(sf::Color(baseStyle.fillColor));
    this->rectangle->setOutlineColor(sf::Color(baseStyle.outlineColor));
    this->rectangle->setOutlineThickness(baseStyle.thickness);

}

void Button::applyHoverStyle() {
    this->rectangle->setFillColor(sf::Color(hoverStyle.fillColor));
    this->rectangle->setOutlineColor(sf::Color(hoverStyle.outlineColor));
    this->rectangle->setOutlineThickness(hoverStyle.thickness);

}

void Button::processEvent(sf::Event& event) {

    switch(event.type) {
        case sf::Event::MouseMoved: 
            if (rectangle->getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                applyHoverStyle();
            } else {
                applyBaseStyle();
            }
            break;
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(*rectangle);
    app::printw(text, x + 5, y + 2, 0x888888ff);
}
