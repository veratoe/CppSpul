#include <SFML/Graphics.hpp>

class app {

    public:
        static void initialize();
        static void update();
        static void draw();
        static void onMouseMoved(sf::Event& event);
        static void onMouseWheelScrolled(sf::Event& event);
        static void onKeyPressed(sf::Event& event);
        static void onMouseButtonPressed(sf::Event& event);

        static void debugView();

        static sf::RenderWindow* window;

};
