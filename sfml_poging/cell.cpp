#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>

#include "cell.h"

using namespace std;

vector<cell*> cell::cells;
vector<cell*> cell::new_cells;

int cell::cell_grid[200][200];

cell::cell(int _x, int _y) {

    x = _x;
    y = _y;

    index = cells.size() + 1;

    cell_grid[x][y] = index;

    destroyed = false;

}

cell::~cell() {

}

void cell::draw(sf::RenderWindow& window) {

    sf::RectangleShape rectangle(sf::Vector2f(4.0f, 4.0f));
    rectangle.setFillColor(sf::Color(0x7aa6c2ff));
    rectangle.setPosition(x * 5, y * 5);
    window.draw(rectangle);
   
}

void cell::findNeighbors() {

    neighbors.clear();

    for (int i = x - 1; i < x + 2; i++) {

        for (int j = y - 1; j < y + 2; j++) {

            if ((i < 0 ) || (j < 0)) {
                continue;
            }

            if (cell_grid[i][j] != 0) {
                neighbors.push_back(cell_grid[i][j]);
            }
        }
    }

}

void cell::update() {

    findNeighbors();

    if (neighbors.size() < 3 && rand() & 1000 > 400) {
        int offsetX = rand() % 3 - 1;
        int offsetY = rand() % 3 - 1;

        if (canCreateCell(x + offsetX, y + offsetY)) {
            cell* c = new cell(x + offsetX, y + offsetY);
            // nieuwe cell cachen om later in de bak te gooien;
            new_cells.push_back(c);
        }
    }

    if (neighbors.size() > 6) {
        kill();
    }
    if (rand() % 15000 < cell::cells.size()) {
        kill();
    }

}

void cell::kill () {

    destroyed = true;
    cell_grid[x][y] = 0;

}

bool cell::canCreateCell(int x, int y) {
    if (x < 0 || y < 0) return false;
    if (x > 160 || y > 100) return false;
    return cell_grid[x][y] == 0;
}