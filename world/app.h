#include <SFML/Graphics.hpp>

class app {

    public:
        static void initialize(std::vector< std::vector<float> >& world);
        static void update();
        static void draw();
        static void onMouseMoved(sf::Event& event);
        static void onMouseWheelScrolled(sf::Event& event);
        static void onKeyPressed(sf::Event& event);

        static sf::RenderWindow* window;

};
