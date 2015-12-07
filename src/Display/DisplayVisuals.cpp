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
    this->setupFluid();
    this->setupDisplayArea();
    
}


void DisplayVisuals::createImageVisuals()
{
    string resourceName = "Dot";
    m_imageVisual = ofPtr<ImageVisual>(new ImageVisual(ofVec3f(0,0,0),resourceName,true));
}


void DisplayVisuals::setupFluid()
{
    ofSetVerticalSync(false);
    
    int drawWidth = 1280*0.1;
    int drawHeight = 720*0.1;
    
    // process all but the density on 16th resolution
    int flowWidth = drawWidth/4;
    int flowHeight = drawHeight/4;
    
    // Flow & Mask
    m_opticalFlow.setup(flowWidth, flowHeight);
    m_velocityMask.setup(drawWidth, drawHeight);
    
    // m_fluid
    #ifdef USE_FASTER_INTERNAL_FORMATS
        m_fluid.setup(flowWidth, flowHeight, drawWidth, drawHeight, true);
    #else
        m_fluid.setup(flowWidth, flowHeight, drawWidth, drawHeight, false);
    #endif
    
    
    // Visualisation
    m_displayScalar.allocate(flowWidth, flowHeight);
    m_velocityField.allocate(flowWidth / 4, flowHeight / 4);
    m_temperatureField.allocate(flowWidth / 4, flowHeight / 4);
    
    // Draw Forces
    m_numDrawForces = 6;
    m_flexDrawForces = new ftDrawForce[m_numDrawForces];
    m_flexDrawForces[0].setup(drawWidth, drawHeight, FT_DENSITY, true);
    m_flexDrawForces[0].setName("draw full res");
    m_flexDrawForces[1].setup(flowWidth, flowHeight, FT_VELOCITY, true);
    m_flexDrawForces[1].setName("draw flow res 1");
    m_flexDrawForces[2].setup(flowWidth, flowHeight, FT_TEMPERATURE, true);
    m_flexDrawForces[2].setName("draw flow res 2");
    m_flexDrawForces[3].setup(drawWidth, drawHeight, FT_DENSITY, false);
    m_flexDrawForces[3].setName("draw full res");
    m_flexDrawForces[4].setup(flowWidth, flowHeight, FT_VELOCITY, false);
    m_flexDrawForces[4].setName("draw flow res 1");
    m_flexDrawForces[5].setup(flowWidth, flowHeight, FT_TEMPERATURE, false);
    m_flexDrawForces[5].setName("draw flow res 2");
    
    m_lastMouse.set(0,0);
    
}


void DisplayVisuals::setupDisplayArea()
{
    m_displayScale = ofPoint(1);
    m_displayOffset = ofPoint(0);
    
    m_displayArea = AppManager::getInstance().getDisplayManager()->getRingArea();
    
    ofLogNotice()<< "DisplayVisuals::setupDisplayArea-> x = " << m_displayArea.x  << ", y = " << m_displayArea.y;
    m_displayArea.width = AppManager::getInstance().getCameraTrackingManager()->getWidth();
    m_displayArea.height = AppManager::getInstance().getCameraTrackingManager()->getHeight();
    
    const ofRectangle& ringArea = AppManager::getInstance().getDisplayManager()->getRingArea();
    m_displayArea.scaleTo(ringArea, OF_SCALEMODE_FILL);
    
    this->calibrateDisplayArea();
}


void DisplayVisuals::update()
{
    this->updateFluid();
}

void DisplayVisuals::updateFluid()
{
    double dt = ofGetLastFrameTime();
    
    m_fluid.addVelocity(m_opticalFlow.getOpticalFlowDecay());
    m_fluid.addDensity(m_velocityMask.getColorMask());
    m_fluid.addTemperature(m_velocityMask.getLuminanceMask());
    
    for (int i=0; i<m_numDrawForces; i++) {
        m_flexDrawForces[i].update();
        if (m_flexDrawForces[i].didChange()) {
            // if a force is constant multiply by deltaTime
            float strength = m_flexDrawForces[i].getStrength();
            if (!m_flexDrawForces[i].getIsTemporary())
                strength *=dt;
            switch (m_flexDrawForces[i].getType()) {
                case FT_DENSITY:
                    m_fluid.addDensity(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_VELOCITY:
                    m_fluid.addVelocity(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_TEMPERATURE:
                    m_fluid.addTemperature(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_PRESSURE:
                    m_fluid.addPressure(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_OBSTACLE:
                    m_fluid.addTempObstacle(m_flexDrawForces[i].getTextureReference());
                default:
                    break;
            }
        }
    }
    
    m_fluid.update();
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
            this->drawFluid();
        }
            break;
            
        case 4:
        {
            this->drawPaintFluid();
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
    ofCircle(ofGetMouseX(),ofGetMouseY(),70);
    ofPopStyle();
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

void DisplayVisuals::drawFluid()
{
    
    bool newFrame = AppManager::getInstance().getCameraTrackingManager()->isNewFrame();
    double dt = ofGetLastFrameTime();
    
    ofFbo cameraFbo = AppManager::getInstance().getCameraTrackingManager()->getCameraFbo();
    
    if (newFrame) {
        m_opticalFlow.setSource(cameraFbo.getTextureReference());
        m_opticalFlow.update(dt);
        
        m_velocityMask.setDensity(cameraFbo.getTextureReference());
        m_velocityMask.setVelocity(m_opticalFlow.getOpticalFlow());
        m_velocityMask.update();
    }
    
    m_fluid.draw(m_calibratedDisplayArea.x, m_calibratedDisplayArea.y, m_calibratedDisplayArea.width, m_calibratedDisplayArea.height);
}

void DisplayVisuals::drawPaintFluid()
{
    ofVec2f mouse;
    
    mouse.set(ofGetMouseX() / (float)ofGetWindowWidth(), ofGetMouseY()  / (float)ofGetWindowHeight());
    ofVec2f velocity = mouse - m_lastMouse;
    
    for (int i=0; i<(m_numDrawForces/2); i++) {
        if (m_flexDrawForces[i].getType() == FT_VELOCITY)
            m_flexDrawForces[i].setForce(velocity);
        m_flexDrawForces[i].applyForce(mouse);
    }
    
    m_lastMouse.set(mouse.x, mouse.y);
    
    double dt = ofGetLastFrameTime();
    for (int i=0; i<m_numDrawForces; i++) {
        m_flexDrawForces[i].update();
        if (m_flexDrawForces[i].didChange()) {
            // if a force is constant multiply by deltaTime
            float strength = m_flexDrawForces[i].getStrength();
            if (!m_flexDrawForces[i].getIsTemporary())
                strength *=dt;
            switch (m_flexDrawForces[i].getType()) {
                case FT_DENSITY:
                    m_fluid.addDensity(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_VELOCITY:
                    m_fluid.addVelocity(m_flexDrawForces[i].getTextureReference(), strength);
                    //particleFlow.addFlowVelocity(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_TEMPERATURE:
                    m_fluid.addTemperature(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_PRESSURE:
                    m_fluid.addPressure(m_flexDrawForces[i].getTextureReference(), strength);
                    break;
                case FT_OBSTACLE:
                    m_fluid.addTempObstacle(m_flexDrawForces[i].getTextureReference());
                default:
                    break;
            }
        }
    }
    
    m_fluid.update();
    
    
    const ofRectangle& ringArea = AppManager::getInstance().getDisplayManager()->getRingArea();
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    m_fluid.draw(0, 0, ofGetWidth(),ofGetHeight());
    //m_fluid.draw(ringArea.x, ringArea.y, ringArea.width,ringArea.height);
    ofPopStyle();
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