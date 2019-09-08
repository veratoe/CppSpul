#include "pathfinding.h"

class Unit {

    public:

        // static spul voor textures?
        static bool hasLoaded;

        enum UnitType { PEASANT, PEASANT2, BISHOP, COUNSELOR, MONK, BANDIT };
        enum Direction { UP, DOWN, LEFT, RIGHT };


        // membersszzz

        Unit(Unit::UnitType m_type);

        void update();
        void setDestination(sf::Vector2i destination);
        void setDirection(Direction direction);
        void draw(sf::RenderTarget& target);

        sf::Vector2i m_gridPosition; // positie in termen van grid
        sf::Vector2f m_position; // positie in pixels

        bool isSelected;


    private:

        UnitType m_type;
        Direction m_direction;

        sf::Vector2f m_textureSize; 
        sf::Vector2f m_texturePosition;
        sf::Vector2i m_destination;

        sf::Texture s_unitTexture;
        sf::Sprite m_sprite;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawDebug();

        bool hasNext;
        bool hasDestination;

        std::vector< Pathfinding::Node > m_nodes;
        Pathfinding::Node m_next;
};

