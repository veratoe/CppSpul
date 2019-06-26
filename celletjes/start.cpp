#include <iostream>

#include "ofMain.h"
#include "app.h"
#include "cell.h"

using namespace std;

int main () {

    //FreeConsole();
    ofSetupOpenGL(800, 600, OF_WINDOW);

    ofRunApp( new App());

}  