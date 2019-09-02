#include <SFML/Graphics.hpp>
#include <vector>

class Pathfinding {

    public: 

        struct Node {
            
            // voor het gemak
            int x;
            int y;

            int f;  // total cost
            int g;  // cost so far
            int h;  // heuristic cost to target

            int status; // 0 == pristine, 1 == open, 2 == closed;

            int fromX;
            int fromY;

        };


        static std::vector< Node > find(Node position, Node destination, std::vector< std::vector<int> >& grid, std::vector< int > passableValues);

};
