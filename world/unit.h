#include "pathfinding.h"

class Unit {

    public:

        // static spul voor textures?
        static bool hasLoaded;




        static const sf::Vector2f UP;
        static const sf::Vector2f DOWN;
        static const sf::Vector2f LEFT;
        static const sf::Vector2f RIGHT;

        enum UnitType { PEASANT, PEASANT2, BISHOP, COUNSELOR, MONK, BANDIT };


        // membersszzz

        Unit(Unit::UnitType m_type);

        void update();
        void setDestination(sf::Vector2i destination);
        void draw(sf::RenderTarget& target);

        sf::Vector2i m_gridPosition; // positie in termen van grid
        sf::Vector2f m_position; // positie in pixels

        bool isSelected;


    private:

        UnitType m_type;

        sf::Vector2f m_direction;
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

