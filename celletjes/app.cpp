#include <iostream>
#include "app.h"
#include "cell.h"

using namespace std;

vector<cell> cells;

void App::setup() {
    ofBackground(244, 230, 120);
    cell c = cell(ofRandom(0, 100), ofRandom(0, 100));
    cells.push_back(c);
}

void App::update() {
    int k = 0;

    for (auto c : cells) {
         
        for (int i = c.x -1; i < c.x +2; i++) {
            for (int j = c.y -1; j < c.y +2; j++) {
                if (i < 0 || j < 0) continue;
                if (ofRandom(0, 100) > 98) {
                    if (cell::grid[i][j] == 0) {
                        cells.push_back(cell (i, j));
                    }
                }
            }

        }
        
        cout << typeid(c).name() << endl;

        if (ofRandom(0, 100) > 99) {
            cells.erase(cells.begin() + k);
        }
        
        k++;

    }

    ofSetColor(123, 0, 0);

}

void App::draw() {

    for (auto cell : cells) {

        ofDrawRectangle(cell.x * 5, cell.y * 5, 5, 5);
    }
    ofDrawBitmapString("current size: " + std::to_string(cells.size()), 30, 30);

}

void App::mouseDragged(int x, int y, int button) {
}

void App::mousePressed(int x, int y, int button) {

}