/*
 *  DisplayVisuals.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#include "ofMain.h"

#include "AppManager.h"
#include "DisplayManager.h"

#include "DisplayVisuals.h"


DisplayVisuals::DisplayVisuals()
{
    //Intentionaly left empty
}


DisplayVisuals::~DisplayVisuals()
{
    //Intentionaly left empty
}


void DisplayVisuals::setup()
{
    m_mode = 3;
    this->createImageVisuals();
    this->setupDisplayArea();
    
}


void DisplayVisuals::createImageVisuals()
{
    string resourceName = "Dot";
    m_imageVisual = ofPtr<ImageVisual>(new ImageVisual(ofVec3f(0,0,0),resourceName,true));
}



void DisplayVisuals::setupDisplayArea()
{
    m_displayScale = ofPoint(1);
    m_displayOffset = ofPoint(0);
    
    m_displayArea = AppManager::getInstance().getDisplayManager()->getDisplayArea();
    
    ofLogNotice()<< "DisplayVisuals::setupDisplayArea-> x = " << m_displayArea.x  << ", y = " << m_displayArea.y;
    m_displayArea.width = AppManager::getInstance().getCameraTrackingManager()->getWidth();
    m_displayArea.height = AppManager::getInstance().getCameraTrackingManager()->getHeight();
    
    const ofRectangle& ringArea = AppManager::getInstance().getDisplayManager()->getDisplayArea();
    m_displayArea.scaleTo(ringArea, OF_SCALEMODE_FILL);
    
    this->calibrateDisplayArea();
}


void DisplayVisuals::update()
{
    //
}


void DisplayVisuals::draw()
{
    this->drawVisuals();
}

void DisplayVisuals::drawVisuals()
{
    switch (m_mode) {
        case 1:
        {
            this->drawCircle();
        }
            break;
            
        case 2:
        {
            this->drawParticle();
        }
             break;
            
        case 3:
        {
            this->drawCamera();
        }
            break;
    }
}

void DisplayVisuals::drawCircle()
{
    // Mouse Circle
    ofSetCircleResolution(100);
    ofPushStyle();
    float hue = fmodf(ofGetElapsedTimef()*10,255);
    ofColor c = ofColor::fromHsb(hue, 255, 255);
    ofSetColor(c);
    ofFill();
    
    ofDrawCircle(ofGetMouseX(),ofGetMouseY(),70);
    ofPopStyle();
    
     //ofLogNotice()<< "DisplayVisuals::mouse-> x = " << ofGetMouseX()  << ", y = " << ofGetMouseY();
}

void DisplayVisuals::drawParticle()
{
    ofPushStyle();
    float hue = fmodf(ofGetElapsedTimef()*10,255);
    int size = 350;
    
    ofColor color = ofColor::fromHsb(hue, 255, 255);
    ofVec3f pos = ofVec3f(ofGetMouseX(), ofGetMouseY());
    m_imageVisual->setColor(color);
    m_imageVisual->setWidth(size);
    m_imageVisual->setHeight(size);
    m_imageVisual->setPosition(pos);
    m_imageVisual->setPosition(pos);
    m_imageVisual->draw();
    
    ofPopStyle();
}

void DisplayVisuals::drawCamera()
{
    
    auto area =  AppManager::getInstance().getDisplayManager()->getDisplayArea();
    auto cameraFbo = AppManager::getInstance().getCameraTrackingManager()->getCameraFbo();
    
    cameraFbo.draw(area);
}


void DisplayVisuals::setOffsetX(float & dx)
{
    m_displayOffset.x = dx;
    this->calibrateDisplayArea();
}

void DisplayVisuals::setOffsetY(float & dy)
{
    m_displayOffset.y = dy;
    this->calibrateDisplayArea();
}

void DisplayVisuals::setScaleX(float & sx)
{
    m_displayScale.x = sx;
    this->calibrateDisplayArea();
}

void DisplayVisuals::setScaleY(float & sy)
{
    m_displayScale.y = sy;
    this->calibrateDisplayArea();
}

void DisplayVisuals::calibrateDisplayArea()
{
    m_calibratedDisplayArea = m_displayArea;
    ofPoint center = m_calibratedDisplayArea.getCenter() + m_displayOffset;
    ofPoint area;
    area.x = m_calibratedDisplayArea.getWidth()*m_displayScale.x;
    area.y = m_calibratedDisplayArea.getHeight()*m_displayScale.y;
    
    
    m_calibratedDisplayArea.setFromCenter(center, area.x, area.y);
    
}