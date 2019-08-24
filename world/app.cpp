#include "app.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

sf::Sprite s;
sf::Texture t;
sf::View view;

std::vector< std::vector<float> > world;

sf::Sprite tiles[12 * 21];

sf::RenderWindow* app::window;

sf::RenderTexture r;

int cameraX, cameraY;
int oldMouseX, oldMouseY, oldMouseWheelDelta;

float zoom = 1.0f;

int getTileMask(std::vector< std::vector<float> >& world, int a, int b) {

    int mask = 1;
    int value = 0; 

    for (int j = 1; j > -2; j--) {
        for (int i = -1; i < 2; i++) {
            if ((a + i < 0 || b + j < 0) || (a + i > world.size() - 1 || b + j > world[i].size() - 1))  {
                mask = mask << 1;
                continue;
            }

            if (i == 0 && j == 0) {
                continue;
            }



            if (world[a + i][b + j] == 1) {
                value = value | mask;
            }

            mask = mask << 1;
        }
    }
    
    return value;

}

void app::initialize(std::vector< std::vector<float> >& world) {


    oldMouseWheelDelta = 0;
    oldMouseX = 0;
    oldMouseY = 0;

    t.loadFromFile("overworld_tileset_grass.png");

    r.create(3200, 3200);

    for (int j = 0; j < 21; j++) {
        for (int i = 0; i < 12; i++) {
            tiles[i * 21 + j].scale(sf::Vector2f(1.0f, -1.0f));
            tiles[j * 12 + i].setTexture(t);
            tiles[j * 12 + i].setTextureRect(sf::IntRect(i*16, j*16, 16, 16));
        }
    }

    int done = false;

    while (!done) {
        done = true;
        for (int j = 0; j < 200; j++) {
            for (int i = 0; i < 200; i++) {
                if (world[i][j] == 0) { 
                    int mask = getTileMask(world, i, j);
                    //if (mask & 26 == 26) printf("wub\n");
                    //if (mask & 90 == 90) printf("wub ");
                    //if ((mask & 90 == 26) || (mask & 90 == 74) || (mask & 90 == 82) || (mask & 90 == 84) || (mask & 90 == 90)) {
                    int bla = (mask & 90);
                    if (bla == 90 || bla == 26 || bla == 74 || bla == 82 || bla == 88 || bla == 66 || bla == 24) {
                        printf("%i ", mask);
                        world[i][j] = 1.0;
                        done = false;
                    }
                } 
            }
        }
    }

    for (int j = 0; j < 200; j++) {
        for (int i = 0; i < 200; i++) {


        

            int mask = 0;
            int index = 61;
            mask = getTileMask(world, i, j);


            if (world[i][j] == 0) {
                if (mask == 1) index = 97;
                if (mask == 4) index = 96;
                if (mask == 32) index = 85;
                if (mask == 128) index = 84;


                if ((mask & 2) && (mask & 8) && !(mask & 16) && !(mask & 64 )) index = 48;
                if ((mask & 2) && !(mask & 8) && (mask & 16) && !(mask & 64 )) index = 50;
                if (!(mask & 2) && (mask & 8) && !(mask & 16) && (mask & 64 )) index = 72;
                if (!(mask & 2) && !(mask & 8) && (mask & 16) && (mask & 64 )) index = 74;

                if (mask ==  16 || mask == 20 || mask == 144 || mask == 148) index = 62;
                if (mask ==  8 || mask == 9 || mask == 40 || mask == 41) index = 60;
                if (mask ==  2 || mask == 3 || mask == 6 || mask == 7) index = 49;
                if (mask ==  64 || mask == 96 || mask == 192 || mask == 224) index = 73;
            } else index = 0;
            
            sf::Sprite s = tiles[ index ];
            s.setPosition(sf::Vector2f(i * 16, j * 16));
            //s.scale(sf::Vector2f(4.0f, 4.0f));
            r.draw(s);
        }
    }

    r.getTexture().copyToImage().saveToFile("wub.png");
}

void app::update() {

    view.reset(sf::FloatRect(cameraX, cameraY, 1024, 768));
    view.zoom(zoom);

}

void app::onMouseMoved(sf::Event& event) {


/*
    if (event.key.code ==  sf::Keyboard::Up) {
        cameraY -= 10;
        if (cameraY < 0) cameraY = 0;
    }

    if (event.key.code ==  sf::Keyboard::Down) {
        cameraY += 10;
        if (cameraY > 400) cameraY = 400;
    }
*/
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        cameraX += -(event.mouseMove.x - oldMouseX);
        cameraY += -(event.mouseMove.y - oldMouseY);
    }

    oldMouseY = event.mouseMove.y;
    oldMouseX = event.mouseMove.x;

    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;
    


}

void app::onMouseWheelScrolled(sf::Event& event) {

    //zoom += event.mouseWheelScroll.delta - oldMouseWheelDelta;
    zoom -= 0.01f * event.mouseWheelScroll.delta;

    oldMouseWheelDelta = event.mouseWheelScroll.delta;
    //printf("%f\n", zoom);
    //printf("%f\n", event.mouseWheelScroll.delta);
}

void app::onKeyPressed(sf::Event& event) {
    
    zoom = 1.0f;

}

void app::draw() {


    sf::Sprite s(r.getTexture());
    //s.scale(sf::Vector2f(4.0f, 4.0f));

    window->setView(view);
    window->draw(s);

}
