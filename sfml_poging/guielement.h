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
        void processEvent(sf::Event& event);
        virtual void draw(sf::RenderWindow& window);

        virtual void applyBaseStyle();
        virtual void applyHoverStyle();

        virtual void onHover();
        virtual void onLeave();
        virtual void onClick(sf::Event& event);
        virtual void onDrag(sf::Event& event);

        sf::Shape* baseElement;
        struct style;

};

#endif /* GUIELEMENT */