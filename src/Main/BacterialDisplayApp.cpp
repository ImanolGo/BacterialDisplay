/*
 *  BacterialDisplayApp.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#include "AppManager.h"

#include "BacterialDisplayApp.h"

//--------------------------------------------------------------
void BacterialDisplayApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void BacterialDisplayApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void BacterialDisplayApp::draw(){
    AppManager::getInstance().draw();
}

void BacterialDisplayApp::exit()
{
    ofLogNotice() <<"BacterialDisplayApp::exit";

}

//--------------------------------------------------------------
void BacterialDisplayApp::keyPressed(int key){

}

//--------------------------------------------------------------
void BacterialDisplayApp::keyReleased(int key){

}

//--------------------------------------------------------------
void BacterialDisplayApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void BacterialDisplayApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void BacterialDisplayApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void BacterialDisplayApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void BacterialDisplayApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void BacterialDisplayApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void BacterialDisplayApp::dragEvent(ofDragInfo dragInfo){

}
