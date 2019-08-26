#include <SFML/Graphics.hpp>

class Unit : public sf::Drawable, public sf::Transformable  {

    public:

        const sf::Vector2f UP = sf::Vector2f(0.0f, -1.0f);
        const sf::Vector2f DOWN = sf::Vector2f(0.0f, 1.0f);
        const sf::Vector2f LEFT = sf::Vector2f(-1.0f, 0.0f);
        const sf::Vector2f RIGHT = sf::Vector2f(1.0f, 0.0f);

        enum UnitType { PEASANT, PEASANT2, BISHOP, COUNSELOR, MONK, BANDIT };

        Unit(UnitType t) : m_type(t) {};

        void update();

        void load() {        

            sf::Vector2f position;

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


    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::VertexArray m_vertices;
        sf::Texture m_unitTexture;

        sf::Vector2f f_position;
        sf::Vector2f size;

        UnitType m_type;
        sf::Vector2f m_direction;

};


void Unit::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_unitTexture;

    // draw the vertex array
    target.draw(m_vertices, states);
}

void Unit::update() {

    float speed = 0.5f;

    sf::Vector2f position = getPosition();

    if (rand() % 10000 > 9950)  {
        speed = (rand() % 10 ) / 10.0f;
        printf("%f", speed);
        switch(rand() % 4) {
            case 0: m_direction = UP; break;
            case 1: m_direction = DOWN; break;
            case 2: m_direction = LEFT; break;
            case 3: m_direction = RIGHT; break;
        }
    }

    // poppetje mooi bijdraaien;
    setScale(sf::Vector2f(m_direction == RIGHT ? 2.0 : -2.0, 2.0));
    setPosition(position + speed * m_direction);

}