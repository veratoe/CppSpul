#include "guielement.h"

using namespace std;

class Slider : guiElement {

    int x;
    int y;
    int width;
    int height;

    guiStyle containerBaseStyle;
    guiStyle containerHoverStyle;
    guiStyle dialBaseStyle;
    guiStyle dialHoverStyle;
    string text;

    //sf::RectangleShape* container;
    sf::RectangleShape* dial;

    void applyBaseStyle();
    void applyHoverStyle();

    public:
        Slider(/* args */int x, int y, int width, int height,
            guiStyle containerBaseStyle, guiStyle containerHoverStyle, 
            guiStyle dialBaseStyle, guiStyle dialHoverStyle);
        ~Slider();
        
        void processEvent(sf::Event& event);
        void draw(sf::RenderWindow& window);

        void onClick(sf::Event& event);

};
