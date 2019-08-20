#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "imgui-SFML.h"
#include <imgui.h>
#include "app.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "Le Wub");
    sf::Clock clock;

    app::window = &window;

    app::initialize();

    ImGui::SFML::Init(window);

    window.setFramerateLimit(60);

    while (window.isOpen()) {

        sf::Event event;
        if (window.pollEvent(event)) {

            ImGui::SFML::ProcessEvent(event);

            
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        ImGui::SFML::Update(window, clock.restart());

        window.clear({ 240, 130, 170 });

        app::processEvent(event);
        app::update();

        ImGui::EndFrame();
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    printf("Klaar!");
    return 0;
}