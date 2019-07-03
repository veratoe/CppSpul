using namespace std;

class cell {

    int x;
    int y;
    int index;
    static int cell_grid[200][200];
    vector<int> neighbors;

    public:

        cell();
        ~cell();
        cell(int _x, int _y);

        void update();
        void kill();

        void draw(sf::RenderWindow& window);
        void findNeighbors();

        static vector<cell*> cells;
        static vector<cell*> new_cells;

        static bool canCreateCell(int x, int y);

        bool destroyed;
        string name;
};