
class Town {
    
    public: 
        Town();

        Town(int x, int y) : m_townCenterX(x), m_townCenterY(y) {}

        void grow();

    private:

        void buildRoad();

        int m_townCenterX, m_townCenterY;
    
};