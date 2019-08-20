#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <chrono>

#include "app.h"
#include "cell.h"
#include "guiManager.h"

sf::RenderWindow* app::window;
sf::Font* app::font;

unsigned int app::timer;
unsigned int app::start_time;

int app::v = 180;
int app::death_rate = 1050;

unsigned int app::getTime() {

    auto now = chrono::system_clock::now();
    auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now).time_since_epoch().count();
    return (unsigned int) now_ms;

}

void app::printw(string s, int x, int y, sf::Uint32 color) {

	sf::Text text;

	text.setFont(*app::font); 
    text.setPosition(x, y);
    text.setString(s);
	text.setCharacterSize(12);
	text.setFillColor(sf::Color(color));
	(*window).draw(text);

}

void app::setup(sf::RenderWindow* window, sf::Font* font) {

    app::window = window;
    app::font = font;

    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int x = rand() % 160;
        int y = rand() % 100;

        if (cell::canCreateCell(x, y)) {
            cell* c = new cell(x, y);
            cell::cells.push_back(c);
        }

    }

    timer = getTime();
    start_time = timer;
}

void app::update() {

    window->clear(sf::Color(0xc1e7ffff));

    if (timer + v < getTime()) {

        // update
        for (auto iterator : cell::cells) {
            iterator->update();
        }

        // possibly delete;
        /*
        auto iterator = cell::cells.begin();
        while (iterator != cell::cells.end()) {
            if ((*iterator)->destroyed == true) {
                delete *iterator;
                iterator = cell::cells.erase(iterator);
            } else {
                ++iterator;
            }
        }
        */

        // nieuw celletjes mergen;
        /*for (auto iterator : cell::new_cells) {
            cell::cells.push_back(iterator);
        }*/

        cell::new_cells.clear();


        timer = getTime();
    }

    for (auto iterator : cell::cells) {
        iterator->draw(*window);
    }

    std::string s = "cellen: " + std::to_string(cell::cells.size());
    app::printw(s, 20, 550, 0x888888ff);

    char r[10];
    sprintf(r, "%.2f", (float) (getTime() - start_time) / 1000);
    string sr = r;

    app::printw("tijd: " + sr, 170, 550, 0x888888ff);

    app::printw("vertraging", 400, 520, 0x888888ff);
    app::printw("sterfte", 400, 550, 0x888888ff);
    app::printw(std::to_string(app::v), 650, 520, 0x888888ff);
    app::printw(std::to_string(app::death_rate), 650, 550, 0x888888ff);
    //guiManager::update();

    window->display();



}