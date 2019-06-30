#include <SFML/Graphics.hpp>
#include "guielement.h"

using namespace std;

class guiManager {

    static sf::RenderWindow* window;

    public:
        static void setup(sf::RenderWindow* window);
        static void processEvent(sf::Event& event);
        static void drawElements();

        static void update();

        static vector<guiElement*> elements;
};