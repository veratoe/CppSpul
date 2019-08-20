#include "app.h"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

sf::RenderWindow* app::window;

sf::CircleShape* element;

sf::Vector2f* speed;
sf::Vector2f* position;

std::vector<float> speeds;

int frameCount = 0;

void app::initialize() {


    element = new sf::CircleShape(20);

    speed = new sf::Vector2f(3.f, 2.f);
    position = new sf::Vector2f(40.f, 50.f);

    element->setPosition(*position);
    element->setFillColor(sf::Color(100, 130, 240));

}

void app::update() {

    frameCount++; 

    speed->x += 0.001f * (400.f - position->x);
    speed->y += 0.002f * (300.f - position->y);

    position->x += 0.0014f * speed->x;
    position->y += 0.0014f * speed->y;

    element->setPosition(*position);

    window->draw(*element);

    if (frameCount > 20) {
        speeds.push_back(position->x);
        frameCount = 0;
    }

    if(speeds.size() > 200) {
        speeds.erase(speeds.begin());
    }

}


void app::processEvent(sf::Event& event) {


    if (event.type == sf::Event::MouseMoved && element->getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
        printf("%i", speeds.size());
        ImGui::BeginTooltip();
        ImGui::Text("Empire Sagus Ship");
        ImGui::Separator();
        ImGui::Text("x: %.2f, y: %.2f",  position->x, position->y);
        ImGui::PlotHistogram("", &speeds[0], speeds.size());
        ImGui::End();

    }

}