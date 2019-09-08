#include <SFML/Graphics.hpp>
#include <cmath>
#include "unit.h"
#include "pathfinding.h"
#include "app.h"

bool Unit::hasLoaded;

Unit::Unit(Unit::UnitType type) {

    m_type = type;


    switch (m_type) {
        case Unit::BISHOP: 
            s_unitTexture.loadFromFile("bishop.png");
            m_texturePosition = sf::Vector2f(0, 1);
            m_textureSize = sf::Vector2f(11, 31);
            break;

        case Unit::PEASANT: 
            s_unitTexture.loadFromFile("peasant.png");
            m_texturePosition = sf::Vector2f(0, 11);
            m_textureSize = sf::Vector2f(15, 21);
            break;

        case Unit::PEASANT2: 
            m_texturePosition = sf::Vector2f(16, 15);
            m_textureSize = sf::Vector2f(17, 20);
            break;
    
        case Unit::COUNSELOR: 
            m_texturePosition = sf::Vector2f(17, 47);
            m_textureSize = sf::Vector2f(12, 17);
            break;

        case Unit::MONK: 
            m_texturePosition = sf::Vector2f(49, 47);
            m_textureSize = sf::Vector2f(15, 16);
            break;

        case Unit::BANDIT: 
            m_texturePosition = sf::Vector2f(67, 79);
            m_textureSize = sf::Vector2f(10, 17);
            break;
    }
    
    //m_sprite.setTexture(s_unitTexture);
    //m_sprite.setTextureRect(sf::IntRect(m_texturePosition.x, m_texturePosition.y, m_textureSize.x, m_textureSize.y));

    m_sprite.setScale(2.f, 2.f);

}



const sf::Vector2f Unit::UP(0.0f, -1.0f);
const sf::Vector2f Unit::DOWN(0.0f, 1.0f);
const sf::Vector2f Unit::LEFT(-1.0f, 0.0f);
const sf::Vector2f Unit::RIGHT(1.0f, 0.0f);

void Unit::drawDebug() {

    // ---- zooi tekenen voor debugView ---------------------

    sf::Text text;
    text.setFont(app::font);
    text.setCharacterSize(15);

    char b[50];
    sprintf(b, "direction: %s", m_direction == UP ? "UP" : m_direction == DOWN ? "DOWN" : m_direction == LEFT ? "LEFT" : "RIGHT");
    text.setString(b);
    text.setPosition(sf::Vector2f(0, 110));
    app::debugOverlay.draw(text);


}

void Unit::update() {
    drawDebug();

    m_gridPosition.x = (int) floor(m_position.x / 32);
    m_gridPosition.y = (int) floor(m_position.y / 32);

    int tileSize = 32;

    if (!hasDestination) {
        return;
    }

    if (m_position.x == m_destination.x * tileSize && m_position.y == m_destination.y * tileSize) {
        hasDestination = false;
        return;
    }

    if (m_position.x == m_next.x * tileSize && m_position.y == m_next.y * tileSize) {
        hasNext = false;
    }


    if (!hasNext && m_nodes.size() > 0) {
        m_next = m_nodes.back();
        m_nodes.pop_back();
        hasNext = true;
    }

    if (m_next.x * 32 != m_position.x) {
        m_direction = m_next.x > m_gridPosition.x ? RIGHT : LEFT;
    }

    if (m_next.y * 32 != m_position.y) {
        m_direction = m_next.y > m_gridPosition.y ? DOWN : UP;
    }


    m_position += m_direction;
}

void Unit::setDestination(sf::Vector2i destination) {
    printf("setting destination\n");

    m_destination = destination;

    hasDestination = true;

    std::vector< std::vector<int> > onzin;

    std::vector< int > values = { 1, 3 };

    m_nodes = Pathfinding::find(
            Pathfinding::Node{ x: m_gridPosition.x, y: m_gridPosition.y, f: 0, g: 0, h: 0, status: 0, fromX: 0, fromY: 0 }, 
            Pathfinding::Node{ x: destination.x, y: destination.y, f: 0, g:0, h: 0, status: 0, fromX: 0, fromY: 0 },
            onzin, values);

    if (m_nodes.size() > 0) {
        hasDestination = true;
        m_next = m_nodes.back();
    }
}

void Unit::draw(sf::RenderTarget& target) {
    int tileSize = 32;
    m_sprite.setPosition(m_position + sf::Vector2f(0, tileSize  - 2.f * m_textureSize.y  ));
    m_sprite.setTexture(s_unitTexture);

    int frameOffset = 0;
    if (hasDestination) {
        frameOffset = ((app::frameCount / 10) % 4 ) * 32;
    }

    if (isSelected) {
        sf::CircleShape circle(20);
        circle.setFillColor({ 0, 0, 0, 100 });
        circle.setScale(1.f, 0.4f);
        circle.setPosition(m_position.x - 8, m_position.y + 20);
        target.draw(circle);
    } else {
        sf::RectangleShape rect(sf::Vector2f(tileSize, tileSize));
        rect.setFillColor({ 0, 0, 0 ,0 });
        rect.setOutlineColor({ 255, 255, 0, 50 });
        rect.setOutlineThickness(2.f);
        rect.setPosition(m_gridPosition.x * tileSize, m_gridPosition.y * tileSize);
        target.draw(rect);
    }

    if (m_direction == LEFT) {
        m_sprite.setTextureRect(sf::IntRect(frameOffset + m_texturePosition.x + m_textureSize.x, m_texturePosition.y, - m_textureSize.x, m_textureSize.y));
    } else {
        m_sprite.setTextureRect(sf::IntRect(m_texturePosition.x + frameOffset, m_texturePosition.y, m_textureSize.x, m_textureSize.y));

    }

    target.draw(m_sprite);
}
