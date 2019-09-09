#include "tile.h"

Tile Tile::operator+(const Tile& rhs) {
    Tile lhs (*this);
    Tile result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}


Tile Tile::operator-(const Tile& rhs) {
    Tile lhs (*this);
    Tile result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}
Tile Tile::operator+=(const Tile& rhs) {
    Tile lhs (*this);
    Tile result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;

}