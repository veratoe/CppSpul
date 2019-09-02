#include <SFML/Graphics.hpp>
#include <cmath>
#include "unit.h"
#include "pathfinding.h"
#include "app.h"


sf::Vector2f f_position;

std::vector< Pathfinding::Node > nodes;

Pathfinding::Node next;
bool hasNext;
bool hasDestination;

sf::Vector2i destination;

sf::Texture m_unitTexture;

Unit::UnitType m_type;

void Unit::load() {        

    sf::Vector2f position;
    sf::Vector2f size; 

    m_unitTexture.loadFromFile("characters.png");

    switch (m_type) {
        case Unit::BISHOP: 
            position = sf::Vector2f(97, 5);
            size = sf::Vector2f(13, 27);
            break;

        case Unit::PEASANT: 
            position = sf::Vector2f(0, 13);
            size = sf::Vector2f(15, 19);
            break;

        case Unit::PEASANT2: 
            position = sf::Vector2f(16, 15);
            size = sf::Vector2f(17, 20);
            break;
    
        case Unit::COUNSELOR: 
            position = sf::Vector2f(17, 47);
            size = sf::Vector2f(12, 17);
            break;

        case Unit::MONK: 
            position = sf::Vector2f(49, 47);
            size = sf::Vector2f(15, 16);
            break;

        case Unit::BANDIT: 
            position = sf::Vector2f(67, 79);
            size = sf::Vector2f(10, 17);
            break;
    }

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(4);

    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(size.x, 0) ;
    m_vertices[2].position = sf::Vector2f(size.x, size.y);
    m_vertices[3].position = sf::Vector2f(0, size.y);

    // define its 4 texture coordinates
    m_vertices[0].texCoords = position;
    m_vertices[1].texCoords = sf::Vector2f(position.x + size.x, position.y);
    m_vertices[2].texCoords = sf::Vector2f(position.x + size.x, position.y + size.y);
    m_vertices[3].texCoords = sf::Vector2f(position.x, position.y + size.y);


    setOrigin(-size.x, 0);

}



const sf::Vector2f Unit::UP(0.0f, -1.0f);
const sf::Vector2f Unit::DOWN(0.0f, 1.0f);
const sf::Vector2f Unit::LEFT(-1.0f, 0.0f);
const sf::Vector2f Unit::RIGHT(1.0f, 0.0f);

void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_unitTexture;

    // draw the vertex array
    target.draw(m_vertices, states);
}

void Unit::drawDebug() {

    // ---- zooi tekenen voor debugView ---------------------

    sf::Vector2f position = getPosition();
    sf::RectangleShape r(sf::Vector2f(32, 32));

    r.setPosition(i_position.x * 32, i_position.y * 32);
    r.setFillColor(sf::Color({ 0, 0, 255, 200 }));
    app::debugLayer.draw(r);

    // WAAR STA IK ECHT
    r.setPosition(position.x, position.y);
    r.setFillColor(sf::Color({ 255, 255, 0, 200 }));
    app::debugLayer.draw(r);

    // WAT IS HET DOEL
    r.setPosition(next.x * 32, next.y * 32);
    r.setFillColor(sf::Color({ 0, 255, 0, 200 }));
    app::debugLayer.draw(r);

    sf::Text text;
    text.setFont(app::font);
    text.setCharacterSize(15);

    char b[50];
    sprintf(b, "position (int): %i, %i", i_position.x, i_position.y);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 30));
    app::debugOverlay.draw(text);

    sprintf(b, "next (int): %i, %i", next.x, next.y);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 50));
    app::debugOverlay.draw(text);

    sprintf(b, "position (f): %.0f, %.0f, ", position.x, position.y);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 70));
    app::debugOverlay.draw(text);

    sprintf(b, "next (f): %i, %i, ", next.x * 32, next.y * 32);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 90));
    app::debugOverlay.draw(text);


    sprintf(b, "direction: %s", m_direction == UP ? "UP" : m_direction == DOWN ? "DOWN" : m_direction == LEFT ? "LEFT" : "RIGHT");
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 110));
    app::debugOverlay.draw(text);

    r.setPosition(destination.x * 32, destination.y * 32);
    r.setFillColor(sf::Color({ 0, 100, 250, 130 }));
    app::debugLayer.draw(r);

}

void Unit::update() {
    drawDebug();

    int tileSize = 32;

    sf::Vector2f position = getPosition();

    if (!hasDestination) {
        return;
    }

    if (position.x == destination.x * tileSize && position.y == destination.y * tileSize) {
        hasDestination = false;
        return;
    }

    if (position.x == next.x * tileSize && position.y == next.y * tileSize) {
        hasNext = false;
    }


    if (!hasNext && nodes.size() > 0) {
        next = nodes.back();
        nodes.pop_back();
        hasNext = true;
    }

    if (next.x * 32 != position.x) {
        m_direction = next.x > i_position.x ? RIGHT : LEFT;
    }

    if (next.y * 32 != position.y) {
        m_direction = next.y > i_position.y ? DOWN : UP;
    }

    if (m_direction == LEFT) {
        setScale(sf::Vector2f(-1.f, 1.f));
    } else {
        setScale(sf::Vector2f(1.f, 1.f));
    }

    i_position.x = (int) floor(position.x / 32);
    i_position.y = (int) floor(position.y / 32);


    setPosition(position +  m_direction);
}

void Unit::setDestination(sf::Vector2i destination) {

    this->destination = destination;

    hasDestination = true;

    std::vector< std::vector<int> > onzin;

    std::vector< int > values = { 1, 3 };

    nodes = Pathfinding::find(
            Pathfinding::Node{ x: i_position.x, y: i_position.y, f: 0, g: 0, h: 0, status: 0, fromX: 0, fromY: 0 }, 
            Pathfinding::Node{ x: destination.x, y: destination.y, f: 0, g:0, h: 0, status: 0, fromX: 0, fromY: 0 },
            onzin, values);

    if (nodes.size() > 0) {
        hasDestination = true;
        next = nodes.back();
    }
}
