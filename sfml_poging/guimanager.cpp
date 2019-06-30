#include "guimanager.h"

vector<guiElement*> guiManager::elements;
sf::RenderWindow* guiManager::window;

void guiManager::setup(sf::RenderWindow* window) {
    guiManager::window = window;
}

void guiManager::processEvent(sf::Event& event) {

    for (auto element: elements) {
        element->processEvent(event);
    }

}

void guiManager::update() {

    drawElements();

}

void guiManager::drawElements() {

    for (auto element: elements) {
        element->draw(*window);
    }

}