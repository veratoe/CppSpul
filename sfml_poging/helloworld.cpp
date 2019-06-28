#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>

#include "app.h"

sf::Font font;

int main() {

	printf("starting application..\n");

	sf::RenderWindow window(sf::VideoMode(800, 600), "WUBMANS POWER", sf::Style::Titlebar);
	if (!font.loadFromFile("vcr.ttf")) {
		printf("er ging iets fout met het laden van het font ..\n");
	}

	app::setup(&window, &font );
	printf("done setting up application..\n");

	//window.setIcon();

	while (window.isOpen()) { 

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			std::cout << "ESC pressed, exiting application ....";
			window.close();
		}

		app::update();
		//window.display();
	}

	window.clear();

	return 0;

}
