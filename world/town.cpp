#include "town.h"
#include "app.h"
#include "layer.h"
#include "tile.h"

std::vector< Tile > roadTiles;

int offsetX, offsetY;

int size;
 
void Town::findTownDimensions() {

    townTop = 999;
    townBottom = 0;
    townLeft = 999;
    townRight = 0;

   for (const auto& tile: roadTiles) {
        if (tile.x > townRight) townRight = tile.x;
        if (tile.x < townLeft) townLeft = tile.x;
        if (tile.y > townBottom) townBottom = tile.y;
        if (tile.y < townTop) townTop = tile.y;
    }


}

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

int neighboringRoadTiles(const Tile& tile) {

    int neighbors = 0;

    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {

            if (i == 0 && j == 0) continue;

            if (tile.x + i < 0) continue;
            if (tile.y + j < 0) continue;
            if (tile.x + i >= app::buildings.size()) continue;
            if (tile.y + j >= app::buildings[i].size()) continue;


            if (app::buildings[ tile.x + i ][ tile.y + j ] == 1) {
                neighbors++;
            }
        }
    }

    return neighbors;

}

int calculateRoadMask(const Tile& tile) {

    int bit = 0;
    int mask = 0;

    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {

            if (i == 0 && j == 0) continue;

            bit = bit == 0 ? 1 : bit << 1;

            if (tile.x + i < 0) continue;
            if (tile.y + j < 0) continue;
            if (tile.x + i >= app::buildings.size()) continue;
            if (tile.y + j >= app::buildings[i].size()) continue;


            if (app::buildings[ tile.x + i ][ tile.y + j ] == 1) {

                mask |= bit;

            }
            
        }
    }

    return mask;

}

void Town::drawDebugView() {

    m_layer->m_texture.clear({ 0, 0, 0, 0 });
    return;

/*
    sf::Text text;
    text.setFont(app::font);
    text.setCharacterSize(15);
    text.setString("sdlfksjflaksdjf");
    text.setPosition(sf::Vector2f(0, 130));
    m_layer->m_texture.draw(text);
    */

   for (int i = townLeft; i <= townRight; i++) {
        for (int j = townTop; j <= townBottom; j++) {
            sf::RectangleShape r(sf::Vector2f(32, 32));
            r.setFillColor({ 0, 0, 255, 100 });
            r.setPosition(i * 32, j * 32);
            m_layer->m_texture.draw(r);
        }
   }
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

bool Town::extendRoad(std::vector< Tile >& ends) {

    int index = rand() % ends.size();
    //printf("index: %i\n", index);

    for (int i = 0; i < ends.size(); i++) {
        if (index == ends.size()) index = 0;

        Tile offset;
                
        switch (calculateRoadMask(ends[index]) & 90) {
            case 2: offset = {0, 1}; break;
            case 8: offset = {1, 0}; break;
            case 16: offset = {-1, 0}; break;
            case 64: offset = {0, -1}; break;
        }

        Tile candidate = ends[index] + offset;

        if (isValidTile( candidate) && app::buildings[ candidate.x ][ candidate.y ] == 0) {
            //printf("growing!\n");
            app::buildings[ candidate.x ][ candidate.y ] = 1;
            app::updateBuildingsMasks();

            return true;
        }

        index++;

    }

}

bool Town::branchRoad(std::vector< Tile >& road) {

    if (rand() % 10 > 5) return false;

    //printf("We gaan proberen te branchen, size: %i\n", (int) road.size());

    enum direction { HORIZONTAL, VERTICAL };

    int dir = road[0].x != road[1].x ? HORIZONTAL : VERTICAL;

    //printf("richting: %s\n", dir == 1 ? "VERTICAL" : "HORIZONTAL");
    Tile offset;
                
    switch (dir) {
        case VERTICAL: (rand() % 2 == 0) ? offset = {1, 0} : offset = {-1, 0}; break;
        case HORIZONTAL: (rand() % 2 == 0) ? offset = {0, 1} : offset = {0, -1}; break;
    } 

    //printf("candidatemask: %i\n", candidateMask);

    int index = rand() % road.size();
    //printf("startIndex: %i\n", index);
    //printf("offset: (%i, %i)\n", offset.x, offset.y);

    for (int i = 0; i < road.size(); i++) {
        if (index == road.size()) index = 0;

        Tile candidate = road[i] + offset;
        int candidateMask = calculateRoadMask(candidate);

        //printf("candidateMask: %i, direction: %i\n", candidateMask, dir);

        if ((dir == VERTICAL && (candidateMask == 41 || candidateMask == 148)) || (dir == HORIZONTAL && (candidateMask == 7 || candidateMask == 224))) {
            //printf("toekie!");
            if (isValidTile(candidate) && app::buildings[ candidate.x ][ candidate.y ] == 0) {
                //printf("branching!!!\n");
                app::buildings[ candidate.x ][ candidate.y ] = 1;
                app::updateBuildingsMasks();
                //printf("succes!\n");
                return true;
            }

        }

        index++;
        
    }

}

void Town::buildRoad() {

    int index;
    
    if (roadTiles.size() > 0) {

        // eindes vinden
        // vind een geschikt stukje van de bestaande wegen
        std::vector< Tile > roadEnds;

        for (auto& tile : roadTiles) {

            //printf("(%i, %i)\n", tile.x, tile.y);

            int mask = calculateRoadMask(tile);
            //printf("%i ", mask);

            if ((mask & 90) == 2 || (mask & 90) == 8 || (mask & 90) == 16 || (mask & 90)  == 64) {
                
                roadEnds.push_back(tile); 

                sf::RectangleShape r(sf::Vector2f(32, 32));
                r.setFillColor({ 0, 0, 255, 100 });
                r.setPosition(tile.x * 32, tile.y * 32);
                m_layer->m_texture.draw(r);

            }

        }

        //printf("%i stukjes wegeinde gevonden\n", (int) roadEnds.size());

        if (roadEnds.size() > 0) {   
            
            // aftakken

            std::vector< std::vector< Tile > > roads;

            for (auto& tile : roadEnds) {

                std::vector< Tile > road;

                Tile offset;

                int length = 0;
                int mask = calculateRoadMask(tile) & 90;

                switch (mask) {
                    case 2: offset = {0, -1}; break;
                    case 8: offset = {-1, 0}; break;
                    case 16: offset = {1, 0}; break;
                    case 64: offset = {0, 1}; break;
                }

                do {

                    road.push_back(tile);
                    tile = tile + offset;

                } while ((calculateRoadMask(tile) & mask) == mask);

                
                if (road.size() > 3) roads.push_back(road);

//                printf("\n");
 //               printf("Lengte weg: %i voor mask %i\n", road.size(), mask);

            }

            //printf("We vonden %i weggetjes\n", (int) roads.size());

            for (auto& road : roads) {
                if (branchRoad(road)) return;
            }

            // einde laten groeien.

          
            if (extendRoad(roadEnds)) return;

        }

        
        /*

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
        */

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




bool Town::buildHouse() {

    if (rand() & 10 > 5) return false;

    if (roadTiles.size() == 0) {
        return false;
    }

    for (int i = townLeft; i < townRight; i++) {
        for (int j = townLeft; j < townRight; j++) {

            int mask = (calculateRoadMask({ i, j }) & 90);

            if (neighboringRoadTiles({ i, j }) > 2) {
                if (isValidTile({ i, j }) && app::buildings[i][j] == 0 ) {
                    app::buildings[i][j]= 192 + (rand() % 4);
                    app::updateBuildingsMasks();
                    return true;
                }
            
            }
        }
    }
}

void Town::grow() {

    findRoadTiles();
    findTownDimensions();
    drawDebugView();

    if (!buildHouse()) {
         buildRoad();
    }


}

