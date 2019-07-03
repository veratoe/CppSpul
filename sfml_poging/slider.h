#include <functional>
#include "guielement.h"

using namespace std;

class Slider : guiElement {

    int x;
    int y;
    int width;
    int height;

    function< void(float) > onChange;

    guiStyle containerBaseStyle;
    guiStyle containerHoverStyle;
    guiStyle dialBaseStyle;
    guiStyle dialHoverStyle;
    string text;

    //sf::RectangleShape* container;
    sf::RectangleShape* dial;

    void applyBaseStyle();
    void applyHoverStyle();
    void setDial(int value);

    public:
        Slider(int x, int y, int width, int height,
            guiStyle containerBaseStyle, guiStyle containerHoverStyle, 
            guiStyle dialBaseStyle, guiStyle dialHoverStyle, function < void(float)> onChange);
        ~Slider();

        void set(float value);
        
        void processEvent(sf::Event& event);
        void draw(sf::RenderWindow& window);

        void onClick(sf::Event& event);
        void onDrag(sf::Event& event);

};
