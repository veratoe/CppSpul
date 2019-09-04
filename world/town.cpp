#include "town.h"
#include "app.h"

struct Tile {
    int x;
    int y;
};

std::vector< Tile > roadTiles;


int offsetX, offsetY;

int size;

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

        // vind een geschikt stukje van de bestaande wegen

        for (int i = 0; i < roadTiles.size(); i++) {
            if (index >= roadTiles.size()) index = 0;
            Tile tile = roadTiles[i];
            int mask = calculateRoadMask(tile);

            if (mask == 2 || mask == 8 || mask == 16 || mask == 64) {

                Tile tiles[4][3] = {
                    {{ -1, 0 }, {1, 0}, {0, 1}},
                    {{ -1, 0 }, {1, 0}, {0, -1}},
                    {{ 1, 0 }, {0, -1}, {0, 1}},
                    {{ -1, 0 }, {0, -1}, {0, 1}}
                };

                int offset;

                if (mask == 2) offset = 0;
                if (mask == 64) offset = 1;
                if (mask == 8) offset = 2;
                if (mask == 16) offset = 3;
                
                int start = rand() % 3;

                printf("start: %i\n", start);

                for (int j = 0; j < 3; j++) {
                    if (start > 2) start = 0;

                    printf("%i -> %i, %i\n", start, tiles[offset][start].x, tiles[offset][start].y);

                    Tile candidate = { tile.x + tiles[offset][start].x, tile.y + tiles[offset][start].y };
                    printf("candidate: (%i, %i)\n", candidate.x, candidate.y);

                    if (isValidTile( candidate) && app::buildings[ candidate.x ][ candidate.y ] == 0) {
                        app::buildings[ candidate.x ][ candidate.y ] = 1;
                        app::updateBuildingsMasks();

                        printf("done!\n");
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
            printf("%i, %i\n", tile.x, tile.y);
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

    printf("check voor huisje: ");

    // vind een geschikt stukje van de bestaande wegen

    for (int i = 0; i < roadTiles.size(); i++) {
        if (index >= roadTiles.size()) index = 0;
        Tile tile = roadTiles[i];
        int mask = calculateRoadMask(tile); 
        printf("%i.%i.%s ", i, mask, ((mask & 2) && (mask & 64)) ? "!" : ".");

        // noord - zuid
        printf("     %i     ", mask);
        if ((mask & 2) && (mask & 64)) {
            printf("NZ!\n");
            

            if (isValidTile({ tile.x + 1, tile.y }) && app::buildings[ tile.x + 1][ tile.y] == 0 ) {
         //       printf("3 stukjes NZ, mask: %i\n", mask);
                printMask(mask);
                printf("mask is %i, rechts ervan\n", mask);
                app::buildings[ tile.x + 1][ tile.y] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

            if (isValidTile({ tile.x - 1, tile.y }) && app::buildings[ tile.x - 1][ tile.y] == 0 ) {
          //      printf("3 stukjes NZ, mask: %i\n", mask);
                printMask(mask);
                printf("mask is %i, links ervan\n", mask);
                app::buildings[ tile.x - 1][ tile.y] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

        }

        // oost - west
        if (mask & 24 == 24) {
            printf("OW!\n");

            if (isValidTile({ tile.x, tile.y + 1 }) && app::buildings[ tile.x ][ tile.y + 1] == 0 ) {
                printMask(mask);
                printf("mask is %i, eronder\n", mask);
                app::buildings[ tile.x][ tile.y + 1] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

            if (isValidTile({ tile.x, tile.y - 1 }) && app::buildings[ tile.x ][ tile.y - 1] == 0 ) {
                printMask(mask);
                printf("mask is %i, erboven\n", mask);
                app::buildings[ tile.x ][ tile.y - 1] = 192 + (rand() % 4);
                app::updateBuildingsMasks();
                return true;
            }

        }

        index++;
    }
        printf("\n");

    return false;
}

void Town::grow() {

    findRoadTiles();

    if (!buildHouse()) {
        buildRoad();
    }

}

Town::Town() {

}