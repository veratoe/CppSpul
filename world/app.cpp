#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "app.h"
#include "noise.h"
#include "tilemap.cpp"
#include "unit.h"
#include "pathfinding.h"

sf::Sprite s;
sf::Texture t;
sf::View app::view;

int app::frameCount;

// https://www.redblobgames.com/maps/terrain-from-noise/
std::vector< std::vector<float> > elevation;
std::vector< std::vector<float> > humidity;
std::vector< std::vector<int> > app::terrain;
std::vector< std::vector<int> > app::buildings;

std::vector< std::vector<int> > terrain_tiles; // de tilemap codes
std::vector< std::vector<int> > buildings_tiles; // de tilemap codes


std::vector< Unit > units;

sf::Font app::font;

sf::Sprite tiles[12 * 21];

int tileMapSize = 16;
int tileSize = 32;

int currentBuild; // wat gaan we bouwen

sf::RenderWindow* app::window;

sf::RenderTexture r;

int worldX = 50;
int worldY = 50;

int cameraX, cameraY;
int oldMouseX, oldMouseY, oldMouseWheelDelta;

float zoom = 1.0f;

TileMap terrain_layer, buildings_layer;

bool debugViewOn = true;
bool buildingBuildings = false;

sf::RenderTexture app::debugLayer;
sf::RenderTexture app::debugOverlay;

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
    terrain_tiles = std::vector< std::vector<int> >(worldY, std::vector<int>(worldX));
    buildings = std::vector< std::vector<int> >(worldY, std::vector<int>(worldX, 8)); // 8 = default transparant
    buildings_tiles = std::vector< std::vector<int> >(worldY, std::vector<int>(worldX, 8)); // 8 = default transparant

    sf::Image image;
    image.create(worldX, worldY);
    
    for (int j = 0; j < worldX; j++) {
        for (int i = 0; i < worldY; i++) {
            image.setPixel(i, j, sf::Color({ (sf::Uint8) (elevation[i][j] * 255) , 0 , 0 }));
        }
    }

    image.saveToFile("elevation.png");

    image.create(worldX, worldY);

    for (int j = 0; j < worldX; j++) {
        for (int i = 0; i < worldY; i++) {
                image.setPixel(i, j, sf::Color({ (sf::Uint8) (humidity[i][j] * 255.0f) , 0 , 0 }));
        }
    }

    image.saveToFile("humidity.png");

    oldMouseWheelDelta = 0;
    oldMouseX = 0;
    oldMouseY = 0;

    t.loadFromFile("overworld_tileset_grass.png");

    int done = false;

    while (!done) {
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

    /*
    *
    *           0 = zee
    *           1 = gras = default?
    *           2 = boom
    *           3 = 
    *           4 = rots?
    *
    */

    for (int j = 0; j < worldY; j++) {
        for (int i = 0; i < worldX; i++) {
            sf::RectangleShape s(sf::Vector2f(1, 1));
            terrain[i][j] = 1; // default = gras

            if (elevation[i][j] < 0.35) {
                terrain[i][j] = 0;
                image.setPixel(i, j, sf::Color({0, 50 , 255 }));
            }

            if (elevation [i][j] > 0.7) {
                //terrain[i][j] = 4;    
                //image.setPixel(i, j, sf::Color({ 200, 100, 80 }));
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

            terrain_tiles[i][j] = index;
        }

    }
    

    terrain_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), terrain_tiles, worldX, worldY);
    buildings_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), buildings_tiles, worldX, worldY);

    terrain_layer.scale(sf::Vector2f(2.0f, 2.0f));
    buildings_layer.scale(sf::Vector2f(2.0f, 2.0f));


    debugLayer.create(3200, 3200);
    debugOverlay.create(window->getSize().x, window->getSize().y);

}

void createUnit() {

    Unit::UnitType type;

    switch(currentBuild) {

            case 1: type = Unit::UnitType::PEASANT; break;
            case 2: type = Unit::UnitType::BISHOP; break;
            //case 1: type = Unit::UnitType::PEASANT2; break;
            //case 3: type = Unit::UnitType::COUNSELOR; break;
            //case 4: type = Unit::UnitType::MONK; break;
            //case 5: type = Unit::UnitType::BANDIT; break;
    }

    int i = (oldMouseX + cameraX) / tileSize;
    int j = (oldMouseY + cameraY) / tileSize;

    Unit u(type);
    u.m_gridPosition = sf::Vector2i(i, j);
    u.m_position = sf::Vector2f(i * tileSize, j * tileSize);

    units.push_back(u);

}

void app::drawDebugView() {

}

float lerp(float a, float b, float f) {
    return a + (b - a) * f;
}

void app::update() {

    frameCount++;
    //frameCount %= 60;

    lerp_CameraX = lerp(lerp_CameraX, cameraX, 0.05);
    lerp_CameraY = lerp(lerp_CameraY, cameraY, 0.05);
    
    view.reset(sf::FloatRect((int) lerp_CameraX, (int) lerp_CameraY, 1024, 768));

    terrain_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), terrain_tiles, worldX, worldY);
    buildings_layer.load("overworld_tileset_grass.png", sf::Vector2u(16, 16), buildings_tiles, worldX, worldY);

    for (auto& unit : units) {
            unit.update();
    }
}

void app::updateBuildingsMasks() {

    int mask;
    int index;

    for (int j = 0; j < worldY; j++) {
        for (int i = 0; i < worldY; i++) {

            index = 8; // default = transparant
        
            // 1 = zandweg
            if (buildings[i][j] == 1) {

                mask = getTileMask(buildings, i, j, 1);
                mask += getTileMask(buildings, i, j, 2);

                index = 7;
                
                // uiteindes
                if ((mask & 2) && !(mask & 8) && !(mask & 16) && !(mask & 64 )) index = 40;
                if (!(mask & 2) && (mask & 8) && !(mask & 16) && !(mask & 64 )) index = 7;
                if (!(mask & 2) && !(mask & 8) && (mask & 16) && !(mask & 64 )) index = 5;
                if (!(mask & 2) && !(mask & 8) && !(mask & 16) && (mask & 64 )) index = 16;


                // rechtdoor zonder uiteinde
                if ((mask & 8) && (mask & 16)) index = 6;
                if ((mask & 2) && (mask & 64)) index = 28;

                // bochtjes
                if ((mask & 2) && (mask & 8) && !(mask & 16) && !(mask & 64 )) index = 43;
                if ((mask & 2) && !(mask & 8) && (mask & 16) && !(mask & 64 )) index = 41;
                if (!(mask & 2) && (mask & 8) && !(mask & 16) && (mask & 64 )) index = 19;
                if (!(mask & 2) && !(mask & 8) && (mask & 16) && (mask & 64 )) index = 17;

                // driesprong
                if ((mask & 2) && (mask & 8) && (mask & 16) && !(mask & 64 )) index = 42;
                if ((mask & 2) && (mask & 8) && !(mask & 16) && (mask & 64 )) index = 31;
                if ((mask & 2) && !(mask & 8) && (mask & 16) && (mask & 64 )) index = 29;
                if (!(mask & 2) && (mask & 8) && (mask & 16) && (mask & 64 )) index = 18;
                
                // vol kruispunt

                if ((mask & 2) && (mask & 8) && (mask & 16) && (mask & 64 )) index = 30;

                buildings_tiles[i][j] = index;

            }

            // 2 = stenen weg
            if (buildings[i][j] == 2) {

                //mask = getTileMask(buildings, i, j, 1);
                mask = getTileMask(buildings, i, j, 2);

                index = 11;

                // voor stenen paden telkens 4 erbij dus dit kan veel simpeler
                
                // uiteindes
                if ((mask & 2) && !(mask & 8) && !(mask & 16) && !(mask & 64 )) index = 44;
                if (!(mask & 2) && (mask & 8) && !(mask & 16) && !(mask & 64 )) index = 11;
                if (!(mask & 2) && !(mask & 8) && (mask & 16) && !(mask & 64 )) index = 9;
                if (!(mask & 2) && !(mask & 8) && !(mask & 16) && (mask & 64 )) index = 20;


                // rechtdoor zonder uiteinde
                if ((mask & 8) && (mask & 16)) index = 10;
                if ((mask & 2) && (mask & 64)) index = 32;

                // bochtjes
                if ((mask & 2) && (mask & 8) && !(mask & 16) && !(mask & 64 )) index = 47;
                if ((mask & 2) && !(mask & 8) && (mask & 16) && !(mask & 64 )) index = 45;
                if (!(mask & 2) && (mask & 8) && !(mask & 16) && (mask & 64 )) index = 23;
                if (!(mask & 2) && !(mask & 8) && (mask & 16) && (mask & 64 )) index = 21;

                // driesprong
                if ((mask & 2) && (mask & 8) && (mask & 16) && !(mask & 64 )) index = 46;
                if ((mask & 2) && (mask & 8) && !(mask & 16) && (mask & 64 )) index = 35;
                if ((mask & 2) && !(mask & 8) && (mask & 16) && (mask & 64 )) index = 33;
                if (!(mask & 2) && (mask & 8) && (mask & 16) && (mask & 64 )) index = 22;
                
                // vol kruispunt

                if ((mask & 2) && (mask & 8) && (mask & 16) && (mask & 64 )) index = 30;

                buildings_tiles[i][j] = index;
            }

        }
    }

}

void app::createBuilding() {

    int i = (oldMouseX + cameraX) / tileSize;
    int j = (oldMouseY + cameraY) / tileSize;

    //printf("x: %i, y: %i\n", i, j);
    switch(currentBuild) {
        case 1: buildings[i][j] = 192; break;
        case 2: buildings[i][j] = 1; break;
        case 3: if (buildings[i][j] == 1) buildings[i][j] = 2; break;
        case 4: 
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 4; y++) {
                    buildings[i + x][j + y] = 4;
                    buildings_tiles[i + x][j + y] = 148 + x + y * 12;
                }
            }
            break;

        case 5: 
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    buildings[i + x][j + y] = 5;
                    buildings_tiles[i + x][j + y] = 144 + x + y * 12;
                }
            }
            break;

        case 6: 
            for (int x = 0; x < 2; x++) {
                for (int y = 0; y < 4; y++) {
                    buildings[i + x][j + y] = 6;
                    buildings_tiles[i + x][j + y] = 151 + x + y * 12;
                }
            }
            break;
    }

    updateBuildingsMasks();

}

void app::onMouseButtonPressed(sf::Event& event) {

    int i = (oldMouseX + cameraX) / tileSize;
    int j = (oldMouseY + cameraY) / tileSize;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        /*
        if (buildingBuildings)
            createBuilding();
            else createUnit();
            */

        bool clickedOnUnit = false;

        printf("linkermuis!: %i, %i\n", i, j);

        for (auto& unit : units) {
            unit.isSelected = false;
        }

        for (auto& unit : units) {
            printf("unit op: %i, %i\n", unit.m_gridPosition.x, unit.m_gridPosition.y);
            if (unit.m_gridPosition.x == i && unit.m_gridPosition.y == j) {
                unit.isSelected = true;
            }
        }

    } 
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {


        for (auto& unit : units) {

            if (unit.isSelected) {
                unit.setDestination(sf::Vector2i(i, j));
            }
        }
    }
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

        case sf::Keyboard::Z: 
            buildingBuildings ? createBuilding() : createUnit();
            break;

        case sf::Keyboard::D: cameraX += 100; break;
        case sf::Keyboard::A: cameraX -= 100; break;
        case sf::Keyboard::W: cameraY -= 100; break;
        case sf::Keyboard::S: cameraY += 100; break;

        case sf::Keyboard::Num0: currentBuild = 0; break;
        case sf::Keyboard::Num1: currentBuild = 1; break;
        case sf::Keyboard::Num2: currentBuild = 2; break;
        case sf::Keyboard::Num3: currentBuild = 3; break;
        case sf::Keyboard::Num4: currentBuild = 4; break;
        case sf::Keyboard::Num5: currentBuild = 5; break;
        case sf::Keyboard::Num6: currentBuild = 6; break;
        case sf::Keyboard::Num7: currentBuild = 7; break;
        case sf::Keyboard::Num8: currentBuild = 8; break;
        
        case sf::Keyboard::Tab: debugViewOn = !debugViewOn; break;

        case sf::Keyboard::F1: buildingBuildings = true; break;
        case sf::Keyboard::F2: buildingBuildings = false; break;
    }


    checkCameraBounds();

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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //createBuilding();
    }

    oldMouseY = event.mouseMove.y;
    oldMouseX = event.mouseMove.x;

    checkCameraBounds();

}

void app::draw() {

    //sf::Sprite s(r.getTexture());
    //s.setPosition(sf::Vector2f(0.0f, 0.0f));
    //s.scale(sf::Vector2f(4.0f, 4.0f));

    window->setView(view);
    window->draw(terrain_layer);
    window->draw(buildings_layer);


    if (debugViewOn) {
    
        debugLayer.display();
        window->draw(sf::Sprite(debugLayer.getTexture()));

        window->setView(window->getDefaultView());

        debugOverlay.display();
        window->draw(sf::Sprite(debugOverlay.getTexture()));

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setString("DEBUG");
        text.setPosition(sf::Vector2f(0, 0));
        window->draw(text);

        window->setView(view);

        debugLayer.clear(sf::Color(0, 0, 0, 0));
        debugOverlay.clear(sf::Color(0, 0, 0, 0));

    }

    for (auto& unit : units) {
        unit.draw(*window);
    }
}
