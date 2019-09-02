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

std::vector< Pathfinding::Node* > findNeighbors(Pathfinding::Node node, bool includeDiagonal, std::vector< std::vector<int> >& grid, std::vector< int > passableValues) {
    return std::vector< Pathfinding::Node* >();
}


void debugView(Pathfinding::Node destination, std::vector< std::vector< Pathfinding::Node > > map) { 

    sf::RenderTexture q;
    q.create(3200, 3200);
    q.clear({ 0, 0, 0, 0});

    sf::RectangleShape r(sf::Vector2f(32, 32));
    r.setFillColor(sf::Color({ 255, 0, 0, 255 }));
    r.setPosition(destination.x * 32, destination.y * 32);
    q.draw(r);
        
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {

            r.setPosition(i * 32, j * 32);

            if (map[i][j].status == 0) {
                r.setFillColor(sf::Color({ 0, 0, 0, 0 }));
                q.draw(r);
            }
            if (map[i][j].status == 1) {
                r.setFillColor(sf::Color({ 00, 250, 0 }));
                q.draw(r);
            }
            if (map[i][j].status == 2) {
                r.setFillColor(sf::Color({ 0, 0, 255 }));
                q.draw(r);
                sf::CircleShape triangle(8, 3);
                triangle.setFillColor(sf::Color({ 240, 230, 203 }));
                triangle.setOrigin(4, 4);
                if (map[i][j].fromX > i) {
                    triangle.rotate(-90);
                }
                if (map[i][j].fromX < i) {
                    triangle.rotate(90);
                }
                if (map[i][j].fromY < j) {
                    triangle.rotate(180);
                }

                triangle.setPosition(i * 32 + 12, j * 32 + 12);

                q.draw(triangle);
            }


        }
    }

    q.display();

    app::debugLayer.draw(sf::Sprite(q.getTexture()));

}

// returns list of nodes
std::vector< Pathfinding::Node > Pathfinding::find(Pathfinding::Node position, Pathfinding::Node destination, std::vector< std::vector<int> >& grid, std::vector< int > passableValues) {

    // dit moet mooier en simpeler kunnen
    std::vector< std::vector< Node > > map( app::terrain.size(), std::vector<Node>( app::terrain[0].size() ));

    for (int a = 0; a < map.size(); a++) {
        for (int b = 0; b < map[a].size(); b++) {
            map[a][b].x = a;
            map[a][b].y = b;
        }

    }

    std::vector< Node > path;

    //printf("pathfinding van (%i, %i) naar (%i, %i)...\n", position.x, position.y, destination.x, destination.y);

    if (!isPassable(app::terrain[destination.x][destination.y], passableValues)) {
        printf("destination is not passable\n");
        return path;
    }
     
    Node* currentNode;

    std::vector< Node* > openNodes, closedNodes;
    std::vector< Node* > neighbors;

    int g = 0;

    bool destinationReached = false;

    openNodes.push_back(&map[position.x][position.y]);

    //printf("sadfasdfsadfasdf\n");

    do {

        // even pauze
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        currentNode = nullptr;
        neighbors.clear();

        // stap 1: vind de most promising node
        for (auto node : openNodes) {
            if (currentNode == nullptr || node->h < currentNode->h) {
                currentNode = node;
            }
        }

        //printf("CurrentNode: (%i, %i)\n", currentNode->x, currentNode->y);

        for (auto it = openNodes.begin(); it != openNodes.end(); it++) {
            if (*it == currentNode) {
                openNodes.erase(it);
                break;
            }
        }

        currentNode->status = 2;

        // stap 2: de buurtjes vinden

        if (currentNode->y - 1 >= 0 && isPassable( app::terrain[ currentNode->x ][ currentNode->y - 1], passableValues)) { 
            neighbors.push_back(&map[ currentNode->x ][ currentNode->y - 1]);
        }

        if (currentNode->y + 1 < map[ currentNode->x ].size() && isPassable( app::terrain[ currentNode->x ][ currentNode->y + 1], passableValues)) { 
            neighbors.push_back(&map[ currentNode->x ][ currentNode->y + 1]);
        }

        if (currentNode->x - 1 >= 0 && isPassable( app::terrain[ currentNode->x - 1 ][ currentNode->y], passableValues)) { 
            neighbors.push_back(&map[ currentNode->x - 1 ][ currentNode->y]);
        }

        if (currentNode->x + 1 < map.size() && isPassable( app::terrain[ currentNode->x + 1 ][ currentNode->y], passableValues)) { 
            neighbors.push_back(&map[ currentNode->x + 1 ][ currentNode->y]);
        }



        for (auto neighbor : neighbors) {
            //printf("Neighbor gevonden: (%i, %i)\n", neighbor->x, neighbor->y);

            int h = abs( neighbor->x - destination.x) + abs (neighbor->y - destination.y);


            switch(neighbor->status) {

                case 2: 
                    // node is closed; negeren;
                    break;

                case 1: 
                    // node is al geopend. kortere weg?
                    if (currentNode->g < neighbor->g) {
                        //printf("We komen langs een reeds geopende node, en moeten updaten\n");
                        neighbor->f = currentNode->g + neighbor->h;
                        neighbor->g = currentNode->g;
                        neighbor->fromX = currentNode->x;
                        neighbor->fromY = currentNode->y;
                    }
                    

                    break; // blabla

                case 0: 
                    neighbor->f = g + h;
                    neighbor->g = g;
                    neighbor->h = h;
                    neighbor->fromX = currentNode->x;
                    neighbor->fromY = currentNode->y;
                    neighbor->status = 1; 
                    openNodes.push_back(neighbor); 
                    break;

            }

        }

        g++;

        if (currentNode->x == destination.x && currentNode->y == destination.y) {
            Node node = *currentNode;
            printf("destination reached\n");

            while (!(node.x == position.x && node.y == position.y)) {
                path.push_back(node);
                node = map[ node.fromX ][ node.fromY ];
            }

            debugView(destination, map);

            return path;

        }

    } while(openNodes.size() > 0);

    printf("geen open nodes meer\n");
    debugView(destination, map);
    return path;

}
