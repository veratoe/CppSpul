#include "pathfinding.h"
#include "app.h"
#include <vector>
#include <cstdlib>
#include <limits.h>
#include <chrono>
#include <thread>


bool isPassable(int value, std::vector< int>& passableValues) {

    for (auto& v : passableValues) {
        if (value == v) return true;
    }   

    //printf("%i not passable!", value);
    return false;

}

std::vector< Pathfinding::Node > findNeighbors(Pathfinding::Node node, bool includeDiagonal, std::vector< std::vector<int> >& grid, std::vector< int > passableValues) {

    std::vector< Pathfinding::Node > nodes;

    if (includeDiagonal) {


    } else {

        // TOP
        if (node.y - 1 >= 0 && isPassable(grid[node.x][node.y - 1], passableValues)) { 
            //printf("TOP\n");
            nodes.push_back({ node.x, node.y - 1 });
        }

        // BOTTOM
        if (node.y + 1 <= grid[node.x].size() && isPassable(grid[node.x][node.y + 1], passableValues)) {
            //printf("BOTTOM\n");
            nodes.push_back({ node.x, node.y + 1 });
        }
        
        // LEFT
        if (node.x - 1 >= 0 && isPassable(grid[node.x - 1][node.y], passableValues)) {
            //printf("LEFT\n");
            nodes.push_back({ node.x - 1, node.y });
        }

        // RIGHT
        if (node.x + 1 <= grid.size() && isPassable(grid[node.x + 1][node.y], passableValues)) {
            //printf("RIGHT\n");
            nodes.push_back({ node.x + 1, node.y });
        }

    }

    return nodes;

}



// returns list of nodes
std::vector< Pathfinding::Node > Pathfinding::find(Node position, Node destination, std::vector< std::vector<int> >& grid, std::vector< int > passableValues) {

    //printf("pathfinding van (%i, %i) naar (%i, %i)...\n", position.x, position.y, destination.x, destination.y);
     
    Pathfinding::Node currentNode = position;
    Pathfinding::Node targetNode = destination;
    Pathfinding::Node bestNode;

    std::vector< Pathfinding::Node > openNodes, closedNodes;
    std::vector< Pathfinding::Node > neighbors;

    int g = 0;

    bool destinationReached = false;

    closedNodes.push_back(currentNode);

    do {
    
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        neighbors = findNeighbors(currentNode, false, app::terrain, passableValues);

        for (auto neighbor : neighbors) {

            int h = abs( neighbor.x - destination.x) + abs (neighbor.y - destination.y);

            neighbor.f = g + h;
            neighbor.g = g;
            neighbor.h = h;

            for (auto node : closedNodes) {
                if (node.x == neighbor.x && node.y == neighbor.y) {
                    neighbor.isClosed = true;
                }

            }

            for (auto node : openNodes) {
                if (node.x == neighbor.x && node.y == neighbor.y) {
                    neighbor.isClosed = true;
                }

            }

            if (!neighbor.isClosed) {
                openNodes.push_back(neighbor);
            }

        }

        bestNode.h = INT_MAX;

        int best_index = 0;

        // find best node
        for (int k = 0; k < openNodes.size(); k++) {
            if (openNodes[k].h < bestNode.h) {
                bestNode = openNodes[k];
                best_index = k;
            }
        }

        // remove from openNodes list;
        if (openNodes.size() > 0) {
            openNodes.erase(openNodes.begin() + best_index);
        }

        // add to closed Nodes list;
        closedNodes.push_back(bestNode);
        
        currentNode = bestNode;

        g++;

        sf::RectangleShape r(sf::Vector2f(32, 32));

        // orig
        r.setPosition(position.x * 32, position.y * 32);
        r.setFillColor(sf::Color({ 0, 255, 0, 255 }));
        app::debugLayer.draw(r);

        // dest
        r.setPosition(destination.x * 32, destination.y * 32);
        r.setFillColor(sf::Color({ 0, 0, 255, 255 }));
        app::debugLayer.draw(r);

        //printf("Nu: (%i, %i), -> h: %i \n ", currentNode.x, currentNode.y, currentNode.h);
            
        for (auto node : openNodes) {
            r.setPosition(node.x * 32, node.y * 32);
            r.setFillColor(sf::Color({ 0, 0, 255, 30 }));
            app::debugLayer.draw(r);
            //app::debugView.display();
        }


        for (auto node : closedNodes) {
            r.setPosition(node.x * 32, node.y * 32);
            r.setFillColor(sf::Color({ 255, 0, 0, 30 }));
            app::debugLayer.draw(r);
        }

        if (currentNode.x == destination.x && currentNode.y == destination.y) {
            //printf("destination reached\n");
            destinationReached = true;
        }

        if (openNodes.size() == 0) {
            //printf("geen open nodes meer\n");
        }

    } while(openNodes.size() > 0 && !destinationReached);


    return openNodes;

}
