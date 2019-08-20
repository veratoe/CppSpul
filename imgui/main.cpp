#include <SFML/Graphics.hpp> 
#include "imgui-SFML.h"
#include "imgui-SFML_export.h"
#include "imgui.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "LELELE");
    sf::Event event;

    ImGui::SFML::Init(window);

    sf::Color bgColor;
    float color[3] = {0.f, 0.2f, 0.3f};
    char windowTitle[255] = "le Ding";

    sf::Clock clock;


    window.setFramerateLimit(60);

    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            printf("ESC pressed, goodbye");
            window.close();
        }

        ImGui::SFML::Update(window, clock.restart());

        ImGui::Begin("Wub");
        ImGui::InputText("Le title", windowTitle, 255);

        ImGui::Text("Tralala");

        ImGui::Separator();


        if (ImGui::ColorEdit3("Background kleurtj", color)) {
            bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
            bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
            bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
        }

        if (ImGui::Button("Doe het trukje")) {
            window.setTitle(windowTitle);
        }

        ImGui::End(); 

        char text[255] = "asdf";

        ImGui::Begin("Window");
        ImGui::InputText("Le Wubel", text, 40);

        ImGui::End();

        window.clear(bgColor);

        ImGui::SFML::Render(window);

        window.display();
    }

    return 0;
}