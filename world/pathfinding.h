#include <SFML/Graphics.hpp>
#include <vector>

class Pathfinding {

    public: 

        struct Node {
            int x;
            int y;
            int f;  // total cost
            int g;  // cost so far
            int h;  // heuristic cost to target

            bool isClosed;

        };


        static std::vector< Node > find(Node position, Node destination, std::vector< std::vector<int> >& grid, std::vector< int > passableValues);

};
