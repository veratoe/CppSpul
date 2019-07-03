#include "guiElement.h"
#include "guiManager.h"

guiElement::guiElement () {

    guiManager::elements.push_back(this);
}

void guiElement::processEvent(sf::Event& event) {

    switch(event.type) {
        case sf::Event::MouseMoved: 
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && baseElement->getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                onDrag(event);

            } else  {
                if (baseElement->getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    onHover();
                } else {
                    onLeave();
                }
            }
            break;

        case sf::Event::MouseButtonPressed:
            if (baseElement->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                onClick(event);
            }
    }

}

void guiElement::onDrag(sf::Event& event) {

    // ...
    // virtual

}

void guiElement::onHover() {
    
    // ...
    // virtual

    applyHoverStyle();

}

void guiElement::onLeave() {

    // ...
    // virtual

    applyBaseStyle();
}

void guiElement::onClick(sf::Event& event) {

    // ...
    // virtual

}

void guiElement::applyHoverStyle() {

    // ...
    // virtual

}

void guiElement::applyBaseStyle() {

    // ...
    // virtual
}

void guiElement::draw(sf::RenderWindow& window) {

    // ...
    // virtual

}