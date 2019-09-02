


class Unit : public sf::Drawable, public sf::Transformable  {

    public:

        static const sf::Vector2f UP;
        static const sf::Vector2f DOWN;
        static const sf::Vector2f LEFT;
        static const sf::Vector2f RIGHT;

        enum UnitType { PEASANT, PEASANT2, BISHOP, COUNSELOR, MONK, BANDIT };

        Unit(UnitType t) : m_type(t) {};

        void update();

        void load();

        void setDestination(sf::Vector2i destination);

        sf::Vector2i i_position;

    private:

        UnitType m_type;

        sf::VertexArray m_vertices;
//        sf::Texture m_unitTexture;
//
//        sf::Vector2f f_position;
//        sf::Vector2f size;
        sf::Vector2f m_direction;

        sf::Vector2i destination;
//

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawDebug();

};

