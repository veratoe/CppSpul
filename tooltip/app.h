#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>

class app {


    public:

        static void initialize();
        static void update();
        static void processEvent(sf::Event& event);

        static sf::RenderWindow* window;

};

#endif