#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "app.h"
#include "noise.h"
#include "tilemap.cpp"

sf::Sprite s;
sf::Texture t;
sf::View view;

// https://www.redblobgames.com/maps/terrain-from-noise/
std::vector< std::vector<float> > elevation;
std::vector< std::vector<float> > humidity;
std::vector< std::vector<int> > terrain;
std::vector< std::vector<int> > buildings;

std::vector< std::vector<int> > world; // de tilemap codes

sf::Font font;

sf::Sprite tiles[12 * 21];

int tileMapSize = 16;
int tileSize = 32;

sf::RenderWindow* app::window;

sf::RenderTexture r;

int worldX = 50;
int worldY = 50;

int cameraX, cameraY;
int oldMouseX, oldMouseY, oldMouseWheelDelta;

float zoom = 1.0f;

TileMap terrain_layer, buildings_layer;

int getTileMask(const std::vector< std::vector<int> >& array,  int a, int b, int tile) {

    int mask = 1;
    int value = 0; 

    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {
            if ((a + i < 0 || b + j < 0) || (a + i > array.size() - 1 || b + j > array[i].size() - 1))  {
                mask = mask << 1;
                continue;
            }

            if (i == 0 && j == 0) {
                continue;
            }

            if (array[a + i][b + j] == tile) {
                value = value | mask;
            }

            mask = mask << 1;
        }
    }
    
    return value;

}

float lerp_CameraX = 0;
float lerp_CameraY = 0;

void checkCameraBounds() {

    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;

    if (cameraX > tileSize * worldX - 1024) cameraX = tileSize * worldX - 1024;
    if (cameraY > tileSize * worldY - 768) cameraY = tileSize * worldY - 768;

    
    
}

void app::initialize() {
    
    font.loadFromFile("vcr.ttf");

    elevation = Noise::generate(worldX, worldY);
    humidity = Noise::generate(worldX, worldY);


    terrain = std::vector< std::vector<int> >(worldY, std::vector<int>(worldX));
    world = std::vector< std::vector<int> >(worldY, std::vector<int>(worldX));
    buildings = std::vector< std::vector<int> >(worldY, std::vector<int>(worldX, 8));

    buildings[10][10] = 2;
    buildings[10][9] = 1;
    buildings[20][10] = 2;

    sf::Image image;
    image.create(worldX, worldY);
    
    for (int j = 0; j < worldX; j++) {
        for (int i = 0; i < worldY; i++) {
            image.setPixel(i, j, sf::Color({ elevation[i][j] * 255 , 0 , 0 }));
        }
    }

    image.saveToFile("elevation.png");

    image.create(worldX, worldY);

    for (int j = 0; j < worldX; j++) {
        for (int i = 0; i < worldY; i++) {
                image.setPixel(i, j, sf::Color({ humidity[i][j] * 255 , 0 , 0 }));
        }
    }

    image.saveToFile("humidity.png");

    oldMouseWheelDelta = 0;
    oldMouseX = 0;
    oldMouseY = 0;

    t.loadFromFile("overworld_tileset_grass.png");

    int done = false;

    while (!done) {
        printf("looping...");
        done = true;
        for (int j = 0; j < worldY; j++) {
            for (int i = 0; i < worldX; i++) {
                if (terrain[i][j] == 0) { 
                    int mask = getTileMask(terrain, i, j, 1);
                    int bla = (mask & 90);
                    if (bla == 90 || bla == 26 || bla == 74 || bla == 82 || bla == 88 || bla == 66 || bla == 24) {
                        terrain[i][j] = 1;
                        done = false;
                    }
                } 
                if (terrain[i][j] == 1) { 
                    int mask = getTileMask(terrain, i, j, 2);
                    int bla = (mask & 90);
                    if (bla == 90 || bla == 26 || bla == 74 || bla == 82 || bla == 88 || bla == 66 || bla == 24) {
                        terrain[i][j] = 2;
                        done = false;
                    }
                }
            }
        }
    }


    image.create(worldX, worldY);

    for (int j = 0; j < worldY; j++) {
        for (int i = 0; i < worldX; i++) {
            sf::RectangleShape s(sf::Vector2f(1, 1));
            terrain[i][j] = 1;
            if (elevation[i][j] < 0.35) {
                terrain[i][j] = 0;
                image.setPixel(i, j, sf::Color({0, 50 , 255 }));
            }

            if (elevation [i][j] > 0.7) {
                terrain[i][j] = 4;    
                image.setPixel(i, j, sf::Color({ 200, 100, 80 }));
            }

            if (elevation[i][j] > 0.4 && elevation[i][j] < 0.6 && humidity[i][j]  < 0.4) {
                terrain[i][j] = 2;
                image.setPixel(i, j, sf::Color({ 200, 200, 80 }));
            }
            
            if (elevation[i][j] > 0.5 && humidity[i][j] > 0.25 && humidity[i][j] < 0.3) {
                    terrain[i][j] = 3;
            }             
            
        }
    }

    image.saveToFile("terrain.png");

    for (int j = 0; j < worldY; j++) {
        for (int i = 0; i < worldX; i++) {

            int index = 61;
            int mask = 0;

            // water
            if (terrain[i][j] == 0) {

                mask = getTileMask(terrain, i, j, 1);

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
                if (mask ==  2 || mask == 3 || mask == 6 || mask == 7) index= 49;
                if (mask ==  64 || mask == 96 || mask == 192 || mask == 224) index = 73;

            } else if (terrain[i][j] == 1) {
                index = 0;   

            } else if (terrain[i][j] == 2) {

                index = 70; // default

                mask = getTileMask(terrain, i, j, 1);

                if ((mask & 2) && (mask & 8) && !(mask & 16) && !(mask & 64 )) index = 57;
                if ((mask & 2) && !(mask & 8) && (mask & 16) && !(mask & 64 )) index = 59;
                if (!(mask & 2) && (mask & 8) && !(mask & 16) && (mask & 64 )) index = 81; 
                if (!(mask & 2) && !(mask & 8) && (mask & 16) && (mask & 64 )) index = 83;
                

                if (mask ==  16 || mask == 20 || mask == 144 || mask == 148) index = 71;
                if (mask ==  8 || mask == 9 || mask == 40 || mask == 41) index = 69;
                if (mask ==  2 || mask == 3 || mask == 6 || mask == 7) index = 58;
                if (mask ==  64 || mask == 96 || mask == 192 || mask == 224) index = 82;
                
            } else if (terrain[i][j] == 3) {
                index = 1;

            } else if (terrain[i][j] == 4) {

                index = 0;

                mask = getTileMask(terrain, i, j, 1);

                if (mask == 1) index = 103;
                if (mask == 4) index = 102;
                if (mask == 32) index = 91;
                if (mask == 128) index = 90;

                if ((mask & 2) && (mask & 8) && !(mask & 16) && !(mask & 64 )) index = 54;
                if ((mask & 2) && !(mask & 8) && (mask & 16) && !(mask & 64 )) index = 56;
                if (!(mask & 2) && (mask & 8) && !(mask & 16) && (mask & 64 )) index = 78;
                if (!(mask & 2) && !(mask & 8) && (mask & 16) && (mask & 64 )) index = 80;
                

                if (mask ==  16 || mask == 20 || mask == 144 || mask == 148) index = 68;
                if (mask ==  8 || mask == 9 || mask == 40 || mask == 41) index = 66;
                if (mask ==  2 || mask == 3 || mask == 6 || mask == 7) index = 55;
                if (mask ==  64 || mask == 96 || mask == 192 || mask == 224) index = 79;
            }

            world[i][j] = index;
        }

    }
    

    terrain_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), world, worldX, worldY);
    buildings_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), buildings, worldX, worldY);

    terrain_layer.scale(sf::Vector2f(2.0f, 2.0f));
    buildings_layer.scale(sf::Vector2f(2.0f, 2.0f));

}

void app::debugView() {

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(10);

    for (int j = 0; j < worldY; j++) {
        for (int i = 0; i < worldX; i++) {
            int index = 0;

            text.setString(std::to_string(world[i][j]));
            text.setPosition(i * tileSize + 4, j * tileSize + 4);
            text.setFillColor(sf::Color({ 255, 255, 255, 90 }));
            window->draw(text);

            text.setString(std::to_string(buildings[i][j]));
            text.setPosition(i * tileSize + 4, j * tileSize + 16);
            text.setFillColor(sf::Color({ 255, 0, 0, 90 }));
            window->draw(text);



            if (buildings[i][j] == 0) {
                continue;
            }

            if (buildings[i][j] == 1) {
                index = 6;
            }

            if (buildings[i][j] == 2) {
                index = 193;
            }

            //sf::Sprite s = tiles[ index ];
            //s.scale(sf::Vector2f(2.0f, 2.0f));
            //s.setPosition(sf::Vector2f(i * tileSize, j * tileSize + 32));
            //r.draw(s);
        }
    }

}

float lerp(float a, float b, float f) {
    return a + (b - a) * f;
}

void app::update() {

    lerp_CameraX = lerp(lerp_CameraX, cameraX, 0.1);
    lerp_CameraY = lerp(lerp_CameraY, cameraY, 0.1);
    
    view.reset(sf::FloatRect((int) lerp_CameraX, (int) lerp_CameraY, 1024, 768));
    //view.zoom(zoom);

    terrain_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), world, worldX, worldY);
    buildings_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), buildings, worldX, worldY);

    //terrain_layer.scale(sf::Vector2f(2.0f, 2.0f));
    //buildings_layer.scale(sf::Vector2f(2.0f, 2.0f));
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        cameraX += -(event.mouseMove.x - oldMouseX);
        cameraY += -(event.mouseMove.y - oldMouseY);
    }

    oldMouseY = event.mouseMove.y;
    oldMouseX = event.mouseMove.x;

    checkCameraBounds();

}

void app::onMouseButtonPressed(sf::Event& event) {

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        int i = (oldMouseX + cameraX) / tileSize;
        int j = (oldMouseY + cameraY) / tileSize;

        printf("x: %i, y: %i\n", i, j);
        buildings[i][j] = 192;

    }
    //app::d//rawWorld();
}

void app::onMouseWheelScrolled(sf::Event& event) {

/*
    //zoom += event.mouseWheelScroll.delta - oldMouseWheelDelta;
    zoom -= 0.01f * event.mouseWheelScroll.delta;

    oldMouseWheelDelta = event.mouseWheelScroll.delta;
    //printf("%f\n", zoom);
    //printf("%f\n", event.mouseWheelScroll.delta);
    */
}

void app::onKeyPressed(sf::Event& event) {
    
    switch(event.key.code) {

        case sf::Keyboard::D: cameraX += 100; break;
        case sf::Keyboard::A: cameraX -= 100; break;
        case sf::Keyboard::W: cameraY -= 100; break;
        case sf::Keyboard::S: cameraY += 100; break;

    }

    checkCameraBounds();

}

void app::draw() {


    //sf::Sprite s(r.getTexture());
    //s.setPosition(sf::Vector2f(0.0f, 0.0f));
    //s.scale(sf::Vector2f(4.0f, 4.0f));

    window->setView(view);
    window->draw(terrain_layer);
    window->draw(buildings_layer);

    //debugView();
}
