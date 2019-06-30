#include "guiElement.h"
#include "guiManager.h"

guiElement::guiElement () {

    guiManager::elements.push_back(this);
}

void guiElement::processEvent(sf::Event& event) {

    // ...
    // virtual

}

void guiElement::draw(sf::RenderWindow& window) {

    // ...
    // virtual

}