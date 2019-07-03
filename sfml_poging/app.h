#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>

class app {

    static sf::RenderWindow* window;
    static sf::Font* font;


    static unsigned int timer;
    static unsigned int start_time;

    public:

        static void printw(std::string s, int x, int y, sf::Uint32 color);
        static void update();
        static void setup(sf::RenderWindow* window, sf::Font* font);

        static unsigned int getTime();
        static int v;
        static int death_rate;
};

#endif