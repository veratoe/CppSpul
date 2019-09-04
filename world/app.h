#include <SFML/Graphics.hpp>
#include <vector>

class app {

    public:
        static void initialize();
        static void update();
        static void draw();
        static void onMouseMoved(sf::Event& event);
        static void onMouseWheelScrolled(sf::Event& event);
        static void onKeyPressed(sf::Event& event);
        static void onMouseButtonPressed(sf::Event& event);


        static sf::RenderWindow* window;

        static sf::RenderTexture debugLayer;
        static sf::RenderTexture debugOverlay;

        static std::vector< std::vector<int> > terrain;
        static std::vector< std::vector<int> > buildings;

        static sf::Font font;
        static sf::View view;

        static int frameCount;

        static void updateBuildingsMasks();

    private:

        static void createBuilding();
        static void drawDebugView();
};
