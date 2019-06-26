#include "ofMain.h"

class App : public ofBaseApp {

    public:
        void setup();
        void draw();
        void update();

        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
};