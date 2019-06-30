#ifndef GUIELEMENT
#define GUIELEMENT

#include <SFML/Graphics.hpp>

struct guiStyle {
    sf::Uint32 fillColor;
    sf::Uint32 outlineColor;
    float thickness;
};

class guiElement {

    public:
        guiElement();
        virtual void processEvent(sf::Event& event);
        virtual void draw(sf::RenderWindow& window);
        struct style;
};

#endif /* GUIELEMENT */