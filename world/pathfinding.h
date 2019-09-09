#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "layer.h"

class Pathfinding {

    public: 

        enum Algorithm { GREEDY, ASTAR };

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


        static Algorithm algorithm;

        static std::vector< Node > find(Node position, Node destination, std::vector< std::vector<int> >& grid, std::vector< int > passableValues);

        static void clearDebugView();
        

    private:

        static Layer* m_layer;

        static void initialize();
        static bool classInitialized;

        static void debugView(Node start, Pathfinding::Node destination, std::vector< std::vector< Pathfinding::Node > > map);

};

#endif // PATHFINDING_H
