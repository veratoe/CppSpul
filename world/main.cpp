#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <vector>

#include "app.h"



int main() {


    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML");

    window.setFramerateLimit(60);

    app::window = &window;
    app::initialize();

    while(window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            if (event.type == sf::Event::MouseMoved) {
                app::onMouseMoved(event);
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                app::onMouseWheelScrolled(event);
            }

            if (event.type == sf::Event::KeyPressed) {
                app::onKeyPressed(event);
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                app::onMouseButtonPressed(event);
            }

        }

        app::update();

        window.clear({ 160, 208, 230});
        app::draw();
        window.display();

    

    }

    return 0;

}   
