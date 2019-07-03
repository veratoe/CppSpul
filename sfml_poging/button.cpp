#include "button.h"
#include <iostream>

Button::Button(int x, int y, int width, int height, string text, guiStyle baseStyle, guiStyle hoverStyle) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->baseElement = new sf::RectangleShape(sf::Vector2f(width, height));
    this->baseElement->setPosition(x, y);

    this->baseStyle = baseStyle;
    this->hoverStyle = hoverStyle;
    this->text = text;

    applyBaseStyle();
}

void Button::applyBaseStyle() {
    this->baseElement->setOutlineColor(sf::Color(baseStyle.outlineColor));
    this->baseElement->setOutlineThickness(baseStyle.thickness);

}

void Button::applyHoverStyle() {
    this->baseElement->setFillColor(sf::Color(hoverStyle.fillColor));
    this->baseElement->setOutlineColor(sf::Color(hoverStyle.outlineColor));
    this->baseElement->setOutlineThickness(hoverStyle.thickness);
}

/* 
void Button::onHover() {
    applyHoverStyle();
}

void Button::onLeave() {
    applyBaseStyle();
}
*/


void Button::onClick() {

}

void Button::draw(sf::RenderWindow& window) {
    window.draw(*baseElement);
    app::printw(text, x + 5, y + 2, 0x888888ff);
}
