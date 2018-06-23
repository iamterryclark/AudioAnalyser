#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofBackground(0);
    
    soundStream.printDeviceList();
    soundStream.setDeviceID(2);
    soundStream.setup(this, 2, 1, 44100, 256, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    audioFeatures.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    audioFeatures.draw();
}

//--------------------------------------------------------------

void ofApp::audioRequested(float * output, int bufferSize, int nChannels){
    audioFeatures.audioRequested(output, bufferSize, nChannels);
}

//--------------------------------------------------------------

void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    audioFeatures.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
