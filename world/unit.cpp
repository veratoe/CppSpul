#include <SFML/Graphics.hpp>
#include "unit.h"
#include "pathfinding.h"
#include "app.h"


sf::Vector2f f_position;

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


    m_vertices[0].position = sf::Vector2f(0 - size.x / 2, 0);
    m_vertices[1].position = sf::Vector2f(size.x / 2, 0) ;
    m_vertices[2].position = sf::Vector2f(size.x / 2, size.y);
    m_vertices[3].position = sf::Vector2f(0 - size.x / 2, size.y);

    // define its 4 texture coordinates
    m_vertices[0].texCoords = position;
    m_vertices[1].texCoords = sf::Vector2f(position.x + size.x, position.y);
    m_vertices[2].texCoords = sf::Vector2f(position.x + size.x, position.y + size.y);
    m_vertices[3].texCoords = sf::Vector2f(position.x, position.y + size.y);

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

void Unit::update() {

    std::vector< std::vector<int> > onzin;

    std::vector< Pathfinding::Node > nodes;

    std::vector< int > values = { 1 };

    if (i_position.x != destination.x || i_position.y != destination.y) {
        nodes = Pathfinding::find(
            Pathfinding::Node{ x: i_position.x, y: i_position.y, f: 0, g: 0, h: 0, isClosed: false }, 
            Pathfinding::Node{ x: destination.x, y: destination.y, f: 0, g:0, h: 0, isClosed: false }, onzin,  values);
    } else {
        setDestination(sf::Vector2i( (rand() % 10) - 5 + i_position.x , (rand() % 10) - 5 + i_position.y));
    }

    if (nodes.size() == 0) {
        return;
    }

    if (nodes[0].x == i_position.x) {
        m_direction = nodes[0].y > i_position.y ? DOWN : UP;
    } else {
        m_direction = nodes[0].x > i_position.x ? RIGHT : LEFT;
    }

    sf::Vector2f position = getPosition();
    setPosition(position + 0.25f * m_direction);

    sf::Text text;
    text.setFont(app::font);
    text.setCharacterSize(20);

    char b[50];
    sprintf(b, "position (float): %.2f, %.2f", position.x, position.y);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 40));
    app::debugOverlay.draw(text);

    sprintf(b, "position (rounded): %i, %i", (int) (position.x / 32), (int) (position.y / 32));
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 60));
    app::debugOverlay.draw(text);

    sprintf(b, "position (int): %i, %i", i_position.x, i_position.y);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 80));
    app::debugOverlay.draw(text);

    sprintf(b, "Doel: %i, %i", destination.x, destination.y);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 100));
    app::debugOverlay.draw(text);

    sprintf(b, "nodes[0]: %i, %i", nodes[0].x, nodes[0].y);
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 120));
    app::debugOverlay.draw(text);


    i_position.x = (int) (position.x / 32); 
    i_position.y = (int) (position.y / 32); 

}

void Unit::setDestination(const sf::Vector2i& destination) {

    this->destination = destination;

}
