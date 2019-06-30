#include <SFML/Graphics.hpp>
#include "guielement.h"
#include "app.h"

using namespace std;

class Button : public guiElement {

    int x;
    int y;
    int width;
    int height;

    guiStyle baseStyle;
    guiStyle hoverStyle;
    string text;

    sf::RectangleShape* rectangle;

    void applyBaseStyle();
    void applyHoverStyle();

    public:

        Button(int x, int y, int width, int height, string text, guiStyle baseStyle, guiStyle hoverStyle);
        void processEvent(sf::Event& event);
        void draw(sf::RenderWindow& window);

};