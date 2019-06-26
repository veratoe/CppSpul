#define TARGET_NO_SOUND
#define OF_SOUND_PLAYER_OPENAL

#include "ofMain.h"
#include "ofApp.h"

int main() {

    ofSetupOpenGL(800, 600, OF_WINDOW);
    ofRunApp(new ofApp());

    cout << "TING";

}