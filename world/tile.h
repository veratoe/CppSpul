#ifndef TILE_H
#define TILE_H

struct Tile {
    int x;
    int y;
    
    Tile operator+(const Tile& rhs);
    Tile operator-(const Tile& rhs);
    Tile operator+=(const Tile& rhs);
};

#endif //TILE_H