#include "town.h"
#include "app.h"
#include "layer.h"

struct Tile {
    int x;
    int y;
};

std::vector< Tile > roadTiles;


int offsetX, offsetY;

int size;

void findRoadTiles() {

    roadTiles.clear();


    for (int i = 0; i < app::buildings.size(); i++ ) {
        for (int j = 0; j < app::buildings[i].size(); j++ ) {
            if (app::buildings[i][j] == 1) {
                roadTiles.push_back(Tile{ i, j });
            }

        }
    }

}


int calculateRoadMask(const Tile& tile) {

    //printf("(%i, %i)\n", tile.x, tile.y);
    
    int bit = 0;
    int mask = 0;

    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {

            if (i == 0 && j == 0) continue;

            bit = bit == 0 ? 1 : bit << 1;

            if (tile.x + i < 0) continue;
            if (tile.y + j < 0) continue;
            //if (tile.x + i >= app::buildings.size()) continue;
            //if (tile.y + j >= app::buildings[i].size()) continue;


            if (app::buildings[ tile.x + i ][ tile.y + j ] == 1) {

                mask |= bit;

            }
            
        }
    }

    return mask;

}

void Town::drawDebugView() {

    m_layer->m_texture.clear({ 0, 0, 0, 0 });

    sf::Text text;
    text.setFont(app::font);
    text.setCharacterSize(15);

    text.setString("sdlfksjflaksdjf");
    text.setPosition(sf::Vector2f(0, 130));
    m_layer->m_texture.draw(text);

    findRoadTiles();

    for (auto& tile : roadTiles) {
        int x = tile.x * 32;
        int y = tile.y * 32;

        int mask = calculateRoadMask({ tile.x, tile.y }) ;
        text.setString(std::to_string(mask));
        text.setPosition(sf::Vector2f( x , y));

        m_layer->m_texture.draw(text);
        if ((mask & 90) == 8 || (mask & 90) == 2 || (mask & 90) == 16 || (mask & 90) == 64) {
            sf::RectangleShape r(sf::Vector2f(32, 32));
            r.setFillColor({ 0, 0, 255, 100 });
            r.setPosition(x, y);
            m_layer->m_texture.draw(r);
        }
    }
}

void printMask(const int mask) {

    int bit = 0;
       
    printf("\n");
    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {
            if (j == 0 && i == 0) { printf("."); continue; }

            bit = bit == 0 ? 1 : bit << 1;

            if (mask & bit) {
                printf("*");
            } else {
                printf(".");
            }

        }
        printf("\n");
    }

    printf("\n");
}

bool inArray(int needle, const std::vector< int >& haystack) {

    for (const auto& value : haystack) {
        if (needle == value) {
            return true;
        }
    }

    // return false 

}

bool isValidTile( Tile tile) {

    if (tile.x < 0) return false;
    if (tile.y < 0) return false;

    if (tile.x >= app::buildings.size()) return false;
    if (tile.y >= app::buildings[ tile.x ].size()) return false;

    return true;

}

void Town::buildRoad() {
    
    if (roadTiles.size() > 0) {

        // willekeurige start index
        int index = rand() % roadTiles.size();
        printf("%i, %i\n", (int) roadTiles.size(), index);

        // vind een geschikt stukje van de bestaande wegen

        for (int i = 0; i < roadTiles.size(); i++) {
            if (index >= roadTiles.size()) index = 0;
            Tile tile = roadTiles[index];
            int mask = calculateRoadMask(tile);

            if ((mask & 90) == 2 || (mask & 90) == 8 || (mask & 90) == 16 || (mask & 90)  == 64) {

                //printf("%i is geschikt\n", i);

                Tile tiles[4][3] = {
                    {{ -1, 0 }, {1, 0}, {0, 1}},
                    {{ -1, 0 }, {1, 0}, {0, -1}},
                    {{ 1, 0 }, {0, -1}, {0, 1}},
                    {{ -1, 0 }, {0, -1}, {0, 1}}
                };

                int offset;

                if ((mask & 90) == 2) offset = 0;
                if ((mask & 90) == 64) offset = 1;
                if ((mask & 90) == 8) offset = 2;
                if ((mask & 90) == 16) offset = 3;
                
                //int start = rand() % 3;
                int start = 2;

                for (int j = 0; j < 3; j++) {
                    if (start > 2) start = 0;

                    Tile candidate = { tile.x + tiles[offset][start].x, tile.y + tiles[offset][start].y };

                    if (isValidTile( candidate) && app::buildings[ candidate.x ][ candidate.y ] == 0) {
                        app::buildings[ candidate.x ][ candidate.y ] = 1;
                        app::updateBuildingsMasks();

                        return;
                    }
                    
                    start++;

                }
            }

            index++;
        }

        // nog steeds niks ...

        bool success;

        Tile tiles[] = {
            
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0}

        };

        int b = 0;

        while (b < 200) {
            b++;
            int a = rand() % 4;

            Tile tile = { roadTiles[0].x + tiles[a].x, roadTiles[0].y + tiles[a].y};
            if (isValidTile(tile) && app::buildings[ tile.x ][ tile.y ] == 0) {
                app::buildings[ tile.x ][ tile.y ] = 1;
                app::updateBuildingsMasks();
                return;
            }
        }

    } else {

        // of plant een random nieuw

        Tile tile;

        do {
            tile.x = m_townCenterX + (rand() % 6) - 3;
            tile.y = m_townCenterY + (rand() % 6) - 3;

        } while (!isValidTile(tile));


        app::buildings[ tile.x ][ tile.y ] = 1;

        app::updateBuildingsMasks();

        return;

    }

}

bool buildHouse() {

    if (roadTiles.size() == 0) {
        return false;
    }

    // willekeurige start index
    int index = rand() % roadTiles.size();

    // vind een geschikt stukje van de bestaande wegen

    for (int i = 0; i < roadTiles.size(); i++) {
        if (index >= roadTiles.size()) index = 0;
        Tile tile = roadTiles[i];
        int mask = calculateRoadMask(tile); 

        // noord - zuid
        if ((mask & 2) && (mask & 64)) {
            

            if (isValidTile({ tile.x + 1, tile.y }) && app::buildings[ tile.x + 1][ tile.y] == 0 ) {
                app::buildings[ tile.x + 1][ tile.y] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

            if (isValidTile({ tile.x - 1, tile.y }) && app::buildings[ tile.x - 1][ tile.y] == 0 ) {
                app::buildings[ tile.x - 1][ tile.y] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

        }

        // oost - west
        if (mask & 24 == 24) {

            if (isValidTile({ tile.x, tile.y + 1 }) && app::buildings[ tile.x ][ tile.y + 1] == 0 ) {
                app::buildings[ tile.x][ tile.y + 1] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

            if (isValidTile({ tile.x, tile.y - 1 }) && app::buildings[ tile.x ][ tile.y - 1] == 0 ) {
                app::buildings[ tile.x ][ tile.y - 1] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

        }

        index++;
    }

    return false;
}

void Town::grow() {

    findRoadTiles();

    if (!buildHouse()) {
        buildRoad();
    }

    drawDebugView();

}

