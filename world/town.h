#include "layer.h"

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

        int m_townCenterX, m_townCenterY;

        Layer* m_layer;
    
};
