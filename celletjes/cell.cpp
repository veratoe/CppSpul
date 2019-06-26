#include <iostream>
#include "cell.h"

using namespace std;

int cell::grid[1000][1000];

cell::cell() {
}

cell::cell(int _x, int _y ) : x(_x), y(_y) {
    
    grid[_x][_y] = 1;
}

void cell::create() {

}

cell::~cell() {

}