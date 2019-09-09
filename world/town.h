#include "layer.h"
#include "tile.h"

class Town {

    public: 

        Town();

        Town(int x, int y) : m_townCenterX(x), m_townCenterY(y) {
            m_layer = new Layer();
        }

        void grow();
        void drawDebugView();

    private:

        void buildRoad();
        bool buildHouse();
        bool branchRoad(std::vector< Tile >& road);
        bool extendRoad(std::vector< Tile >& road);

        void findTownDimensions();

        int m_townCenterX, m_townCenterY;

        int townLeft, townRight, townTop, townBottom;

        Layer* m_layer;
    
};
