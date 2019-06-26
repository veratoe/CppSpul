#include "ofApp.h"

// ofApp::ofApp () {}
// ofApp::~ofApp () {}

int step;

void ofApp::setup() {

    ofBackground(0, 0, 0);
    ofSetWindowTitle("Le wubber");
    step = 15;
}

void ofApp::update() {

}

void ofApp::draw() {

    float hue = fmodf(ofGetElapsedTimef() * 10, 255);


    for (int i = 0; i < ofGetWidth(); i += step) {
        
        for (int j = 0; j < ofGetWidth(); j += step) {
            ofColor c;
            c.setHsb(hue, ofMap(i, 0, ofGetWidth(), 0, 255), ofMap(j, ofGetHeight(), 0, 0, 255));
            ofSetColor(c);
            ofDrawRectangle(i, j, step - 1, step - 1);
        }
    }

    ofSetHexColor(0xffffff);
    ofDrawBitmapString("Dit is fakken vet!!", 200, 200);
    ofDrawBitmapString("De stapgrootte is nu " + std::to_string(step), 200, 220);


}

void ofApp::keyPressed(int k) {

    if (k == 'w') {
        cout << "WUBWUWBUWB";
        step++;
    }

    if (k == 's') {
        cout << "WUBWUWBUWB";
        step--;
        if (step < 0) step = 0;
    }
}
