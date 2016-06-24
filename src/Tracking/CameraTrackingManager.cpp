/*
 *  CameraTrackingManager.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#include "AppManager.h"

#include "CameraTrackingManager.h"

const int CameraTrackingManager::CAMERA_WIDTH = 640;
const int CameraTrackingManager::CAMERA_HEIGHT = 480;

CameraTrackingManager::CameraTrackingManager(): Manager()
{
    //Intentionally left empty
}


CameraTrackingManager::~CameraTrackingManager()
{
    ofLogNotice() <<"CameraTrackingManager::Destructor";
}


void CameraTrackingManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"CameraTrackingManager::initialized";
    
    Manager::setup();
    
    m_hueColor = ofColor::red;
    m_hueColor.a = 50;
    m_hueChangeRate = 1; //One Hue change every second
    this->setupCamera();
}

void CameraTrackingManager::setupCamera()
{
    ofLogNotice() <<"CameraTrackingManager::setupCamera";
    
    m_cameraFbo.allocate(CAMERA_WIDTH, CAMERA_HEIGHT, GL_RGBA);
    m_cameraFbo.begin(); ofClear(0); m_cameraFbo.end();
    
    m_cameraArea.width = ofGetWidth()*0.25;
    m_cameraArea.height = m_cameraArea.width*CAMERA_HEIGHT/CAMERA_WIDTH;
    m_cameraArea.x = ofGetWidth()*0.75 -  m_cameraArea.width*0.5;
    m_cameraArea.y = ofGetHeight()*0.25 -  m_cameraArea.height*0.5;
    
    
    #if defined( TARGET_LINUX_ARM )
        ofLogNotice() <<"CameraTrackingManager::setupCamera-> Linux Target";
    
        ofLogNotice() <<"CameraTrackingManager::setupCamera->  ofSetLogLevel  ofThread";
        ofSetLogLevel("ofThread", OF_LOG_ERROR);
    
       // ofLogNotice() <<"CameraTrackingManager::setupCamera-> setup console";
        //allows keys to be entered via terminal remotely (ssh)
    
        ofLogNotice() <<"CameraTrackingManager::setupCamera-> setup filters";

        //m_filterCollection.setup("Pastel");
    
    
        ofLogNotice() <<"CameraTrackingManager::setupCamera-> camera settings";
        m_omxCameraSettings.width = CAMERA_WIDTH;
        m_omxCameraSettings.height = CAMERA_HEIGHT;
        m_omxCameraSettings.framerate = 30;
        m_omxCameraSettings.isUsingTexture = true;
        m_omxCameraSettings.doRecording = false;   //default false
    
        ofLogNotice() <<"CameraTrackingManager::setupCamera-> video grabber";
        m_videoGrabberPi.setup(m_omxCameraSettings);
        //m_videoGrabberPi.applyImageFilter(m_filterCollection.getNextFilter());
    
    #else
    
        ofLogNotice() <<"CameraTrackingManager::OS X target";
        m_videoGrabber.setDeviceID(0);
        m_videoGrabber.setDesiredFrameRate(60);
        m_videoGrabber.initGrabber(CAMERA_WIDTH,CAMERA_HEIGHT);
    
    #endif
    
}

void CameraTrackingManager::update()
{
    this->updateCamera();
    //this->updateHue();
}

void CameraTrackingManager::updateCamera()
{
    
    #if defined( TARGET_LINUX_ARM )
    
       // m_videoTexture.loadData(m_videoGrabberPi.getPixels(), m_omxCameraSettings.width, m_omxCameraSettings.height, GL_RGBA);
   
    #else
    
        m_videoGrabber.update();
    
    #endif
}

void CameraTrackingManager::updateHue()
{
    float value = fmodf(ofGetElapsedTimef()/m_hueChangeRate,255);
    AppManager::getInstance().getGuiManager()->setHue(value/255.0);
}


void CameraTrackingManager::draw()
{
    this->drawCamera();
    //ofSetColor(ofColor::white);
    //m_videoGrabber.draw(m_cameraFbo.getWidth(), 0, -m_cameraFbo.getWidth(), m_cameraFbo.getHeight() );
    //m_videoGrabber.draw(0, 0, -m_cameraFbo.getWidth(), m_cameraFbo.getHeight());
}

void CameraTrackingManager::drawCamera()
{
    // Blink the led everytime there is a new frame
    ofPushStyle();
    //ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    m_cameraFbo.begin();
    
    ofSetColor(ofColor::white);
    
       
    #if defined( TARGET_LINUX_ARM )
        m_videoGrabberPi.getTextureReference().draw(m_cameraFbo.getWidth(), 0, -m_cameraFbo.getWidth(), m_cameraFbo.getHeight() );
    #else
        m_videoGrabber.draw(m_cameraFbo.getWidth(), 0, -m_cameraFbo.getWidth(), m_cameraFbo.getHeight() );
    #endif
    
    
    //this->drawHueColor();
    
    m_cameraFbo.end();
    
    //m_cameraFbo.draw(m_cameraArea.x,m_cameraArea.y,m_cameraArea.width,m_cameraArea.height);
   // if(m_showCamera){
        
        m_cameraFbo.draw(m_cameraArea.x,m_cameraArea.y,m_cameraArea.width,m_cameraArea.height);
        //m_videoGrabber.draw(m_cameraFbo.getWidth(), 0, -m_cameraFbo.getWidth(), m_cameraFbo.getHeight() );
    //}
    ofPopStyle();
}

void CameraTrackingManager::drawHueColor()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(m_hueColor);
    ofFill();
    ofDrawRectangle(0,0,m_cameraFbo.getWidth(), m_cameraFbo.getHeight());
    ofDisableAlphaBlending();
    ofPopStyle();
    
}



//--------------------------------------------------------------
void CameraTrackingManager::onHueChange(float & value){
    m_hueColor.setHue(value*255);
}

//--------------------------------------------------------------
void CameraTrackingManager::onHueAlphaChange(float & value){
    m_hueColor.a = value*255;
}

//--------------------------------------------------------------
void CameraTrackingManager::onHueChangeRate(float & value){
    m_hueChangeRate = value;
}

