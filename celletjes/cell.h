class cell {

    public:
        cell();
        cell(int x, int y);
        ~cell();
        int x;
        int y;

        static int grid[1000][1000];

        void create();

};