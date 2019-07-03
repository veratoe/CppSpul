#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>

#include "app.h"
#include "button.h"
#include "slider.h"
#include "guimanager.h"

sf::Font font;

int main() {

	printf("starting application..\n");

	sf::RenderWindow window(sf::VideoMode(800, 600), "WUBMANS POWER", sf::Style::Titlebar);
	if (!font.loadFromFile("vcr.ttf")) {
		printf("er ging iets fout met het laden van het font ..\n");
	}

	guiStyle buttonBaseStyle = {
		0xddddddff, // fillColor,
		0xaaaaaaff, // borderColor
		2 // thickness
	};

	guiStyle buttonHoverStyle = {
		0xeeeeeeff, // fillColor,
		0xbbbbbbff, // borderColor
		2 // thickness
	};

	guiStyle dialBaseStyle = {
		0xff8888ff, // fillColor,
		0xbbbbbbff, // borderColor
		0 // thickness
	};

	guiStyle dialHoverStyle = {
		0xffaaaaff, // fillColor,
		0xbbbbbbff, // borderColor
		0 // thickness
	};

	Button button(500, 500, 100, 20, "Explode", buttonBaseStyle, buttonHoverStyle);

	Slider slider(500, 550, 100, 20, buttonBaseStyle, buttonHoverStyle, dialBaseStyle, dialHoverStyle);

	app::setup(&window, &font );
	printf("done setting up application..\n");

	guiManager::setup(&window);
	printf("done setting up the GUI manager..\n");


	//window.setIcon();

	while (window.isOpen()) { 

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			guiManager::processEvent(event);

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			std::cout << "ESC pressed, exiting application ....";
			window.close();
		}

		app::update();
	}

	window.clear();

	return 0;

}
