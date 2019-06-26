#include "ofApp.h"

ofVideoGrabber grabber;
ofPixels pixels;
ofTexture texture;
bool invert;

void ofApp::setup() {


/* 
    vector<ofVideoDevice> devices = grabber.listDevices(); 
 
    cout << devices.size();

    for (size_t i = 0; i < devices.size(); i++) {
        if (devices[i].bAvailable) {
        }

    }
    */

    grabber.setup(64, 48);
}

void ofApp::update() {

    grabber.update();
    pixels = grabber.getPixels();
    for (size_t i = 0; i < pixels.size(); i++) {
        if (invert) {
            pixels[i] = 255 - pixels[i];
        }
    }

    texture.loadData(pixels);


}

void ofApp::draw() {
    ofSetHexColor(0xffffff);

    texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    texture.draw(0, 0, 800, 600);

}

void ofApp::keyPressed(int key) {

    if (key == 'w') {
        invert = !invert;
    }
}