#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <vector>

#include "app.h"
#include "noise.h"


int worldX = 200;
int worldY = 200;

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML");

    app::window = &window;

    std::vector< std::vector<float> > world;

    world  = Noise::generate(worldX, worldY);
    app::initialize(world);

    sf::RenderTexture r;

    r.create(worldX, worldY);
    r.clear({ 0, 0, 255 });

    for (int j = 0; j < worldX; j++) {
        for (int i = 0; i < worldY; i++) {
            if (world[i][j] == 1) {
                sf::RectangleShape s(sf::Vector2f(1, 1));
                if (world[i][j] == 1) {
                    s.setFillColor(sf::Color({100 ,200 ,100 }));
                }
                s.setPosition(sf::Vector2f(i, j));
                r.draw(s);
            }
        }
    }

    r.getTexture().copyToImage().saveToFile("noise.png");
    


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

        }

        app::update();

        window.clear({ 160, 208, 230});
        app::draw();
        window.display();

    

    }

    printf("balbla");
    return 0;


}   
