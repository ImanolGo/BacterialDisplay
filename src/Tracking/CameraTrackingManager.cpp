/*
 *  CameraTrackingManager.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#include "AppManager.h"

#include "ofxPS3EyeGrabber.h"

#include "CameraTrackingManager.h"

using namespace ofxCv;
using namespace cv;

const int CameraTrackingManager::CAMERA_WIDTH = 640; //320
const int CameraTrackingManager::CAMERA_HEIGHT = 480; //240

CameraTrackingManager::CameraTrackingManager(): Manager(), m_numberPixels(16)
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
    
    this->setupCamera();
    this->setupOpenCv();
    this->setupShaders();
}

void CameraTrackingManager::setupCamera()
{
    ofLogNotice() <<"CameraTrackingManager::setupCamera";
    
    m_cameraFbo.allocate(CAMERA_WIDTH, CAMERA_HEIGHT, GL_RGB);
    m_cameraFbo.begin(); ofClear(0); m_cameraFbo.end();
    
    m_cameraArea.width = ofGetWidth()*0.25;
    m_cameraArea.height = m_cameraArea.width*CAMERA_HEIGHT/CAMERA_WIDTH;
    m_cameraArea.x = ofGetWidth()*0.75 -  m_cameraArea.width*0.5;
    m_cameraArea.y = ofGetHeight()*0.25 -  m_cameraArea.height*0.5;
    
    m_numberPixels = 16;
    
    
    
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
    m_omxCameraSettings.enableTexture = true;
    m_omxCameraSettings.enablePixels = true;
    
    ofLogNotice() <<"CameraTrackingManager::setupCamera-> video grabber";
    m_videoGrabberPi.setup(m_omxCameraSettings);
    //m_videoGrabberPi.applyImageFilter(m_filterCollection.getNextFilter());
    
    m_camMatPi = cv::Mat(cvSize(m_omxCameraSettings.width,
                                m_omxCameraSettings.height),
                         CV_8UC4,
                         m_videoGrabberPi.getPixels(),
                         cv::Mat::AUTO_STEP);
    
#else
    
    ofLogNotice() <<"CameraTrackingManager::OS X target";
    #if defined( PSE3 )
        m_videoGrabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
    #else
        m_videoGrabber.setDeviceID(0);
    #endif
    
        //m_videoGrabber.setDesiredFrameRate(60);
    m_videoGrabber.initGrabber(CAMERA_WIDTH,CAMERA_HEIGHT);

    
#endif
    
}

void CameraTrackingManager::setupOpenCv()
{
    m_cropped.allocate(CAMERA_WIDTH,CAMERA_HEIGHT, OF_IMAGE_COLOR);
    
    m_trackingFbo.allocate(CAMERA_WIDTH,CAMERA_HEIGHT,GL_RGB);
    m_trackingFbo.begin(); ofClear(0); m_trackingFbo.end();
    
    m_contrastFbo.allocate(CAMERA_WIDTH,CAMERA_HEIGHT,GL_RGB);
    m_contrastFbo.begin(); ofClear(0); m_contrastFbo.end();
    
    m_pixelatedFbo.allocate(CAMERA_WIDTH,CAMERA_HEIGHT,GL_RGB);
    m_pixelatedFbo.begin(); ofClear(0); m_pixelatedFbo.end();
    
    
    m_objectFinder.setup(ofToDataPath("xmls/haarcascade_frontalface_alt2.xml"));
    m_objectFinder.setPreset(ObjectFinder::Accurate);
    
    
//    m_roi.x = 0;
//    m_roi.y = 0;
//    m_roi.width = CAMERA_WIDTH;
//    m_roi.height = CAMERA_HEIGHT;
    
    
        m_roi.width = CAMERA_HEIGHT*0.2;
        m_roi.height = CAMERA_WIDTH*0.2;
        m_roi.x = CAMERA_WIDTH*0.5  -  m_roi.width *0.5;
        m_roi.y = CAMERA_HEIGHT*0.5 - m_roi.height*0.5;
    
}

void CameraTrackingManager::setupShaders()
{
    #ifdef TARGET_OPENGLES
        m_pixelateShader.load("shaders/shadersES2/pixelate");
        m_contrastShader.load("shaders/shadersES2/contrast");
    #else
        if(ofIsGLProgrammableRenderer()){
            m_pixelateShader.load("shaders/shadersGL3/pixelate");
            m_contrastShader.load("shaders/shadersGL3/contrast");
        }else{
            m_pixelateShader.load("shaders/shadersGL2/pixelate");
            m_contrastShader.load("shaders/shadersGL2/contrast");
        }
    #endif
    
  
}

void CameraTrackingManager::update()
{
    this->updateCamera();
    this->updateOpenCv();
}

void CameraTrackingManager::updateCamera()
{
    
#if defined( TARGET_LINUX_ARM )
    
    // m_videoTexture.loadData(m_videoGrabberPi.getPixels(), m_omxCameraSettings.width, m_omxCameraSettings.height, GL_RGBA);
    
#else
    
    m_videoGrabber.update();
    
#endif
}


void CameraTrackingManager::updateOpenCv()
{
    
    
#if defined( TARGET_LINUX_ARM )
    
    //m_finder.findHaarObjects(m_videoGrabberPi.getPixels());
    if(m_videoGrabberPi.isFrameNew())
    {
        m_objectFinder.update(m_camMatPi);
        if(m_objectFinder.size() > 0) {
            m_roi = toCv(m_objectFinder.getObject(0));
            Mat croppedCamMat(m_camMatPi, m_roi);
            resize(croppedCamMat, m_cropped);
            m_cropped.update();
        }
    }
    
#else
    
    if(m_videoGrabber.isFrameNew()) {
        //m_objectFinder.update(m_videoGrabber);
        
        if(m_objectFinder.size() > 0) {
            
            float speed = 0.2;
            cv::Rect  rect = toCv(m_objectFinder.getObject(0));
            m_roi.x += (rect.x - m_roi.x)*speed;
            m_roi.y += (rect.y - m_roi.y)*speed;
            m_roi.width += (rect.width - m_roi.width)*speed;
            m_roi.height += (rect.height - m_roi.height)*speed;
            
            //m_roi = toCv(m_objectFinder.getObject(0));
        }
        
        Mat camMat = toCv(m_videoGrabber);
        Mat croppedCamMat(camMat, m_roi);
        resize(croppedCamMat, m_cropped);
        m_cropped.update();
    }
    
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
    this->drawTracking();
    
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
    
    ofNoFill();
    
    ofPushStyle();
    ofTranslate(m_cameraFbo.getWidth(), 0);
    ofScale(-1, 1);
    ofDrawRectangle(m_roi.x, m_roi.y, m_roi.width, m_roi.height);
    ofPopStyle();
    //this->drawHueColor();
    
    m_cameraFbo.end();
    

    //m_cameraFbo.draw(m_cameraArea.x,m_cameraArea.y,m_cameraArea.width,m_cameraArea.height);
    if(m_showCamera){
        
        //m_grayDiff.draw(m_cameraArea.x,m_cameraArea.y,m_cameraArea.width,m_cameraArea.height);
        
        //m_videoGrabber.draw(m_cameraFbo.getWidth(), 0, -m_cameraFbo.getWidth(), m_cameraFbo.getHeight() );
        
        
    }
    ofPopStyle();
    
    m_contrastFbo.begin();
        m_contrastShader.begin();
        // m_contrastShader.setUniform4f("range", 0.05, 1, 0.5, 1.0);
            m_contrastShader.setUniform4f("range", m_contrastRange);
            m_cropped.draw(0,0);
        m_contrastShader.end();
    m_contrastFbo.end();
    
    m_pixelatedFbo.begin();
        m_pixelateShader.begin();
        // m_contrastShader.setUniform4f("range", 0.05, 1, 0.5, 1.0);
        m_pixelateShader.setUniform2f("size", CAMERA_WIDTH/m_numberPixels,CAMERA_HEIGHT/m_numberPixels);
        m_contrastFbo.draw(0,0);
        m_pixelateShader.end();
    m_pixelatedFbo.end();
    
    
    
    m_cameraFbo.draw(m_cameraArea.x -m_cameraArea.width*0.5 ,m_cameraArea.y,m_cameraArea.width,m_cameraArea.height);
    
    m_contrastFbo.draw(m_cameraArea.x + m_cameraArea.width*1.5 + 10 ,m_cameraArea.y,-m_cameraArea.width,m_cameraArea.height);
    
}


void CameraTrackingManager::drawTracking()
{
    this->drawROI();
}

void CameraTrackingManager::drawROI()
{
    m_trackingFbo.begin();
    
    //ofClear(0);
    //m_grayImage.drawROI(0,0, m_trackingFbo.getWidth(), m_trackingFbo.getHeight());
    
    ofPushMatrix();
    ofTranslate(m_cameraFbo.getWidth(), 0);
    ofScale(-1, 1);
    m_pixelatedFbo.draw(0, 0, m_trackingFbo.getWidth(), m_trackingFbo.getHeight());
    ofPopMatrix();
    
    m_trackingFbo.end();
    
    float f = ofMap(0, -1, 1, 64, 127);
    
}

void CameraTrackingManager::drawGrayDiff()
{
    ofEnableAlphaBlending();
    m_trackingFbo.begin();
    
    ofPushStyle();
    
    ofFill();
    ofSetColor(0,0,0,8);
    ofDrawRectangle(0,0,m_trackingFbo.getWidth(),m_trackingFbo.getHeight());
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255,255,255);
    //m_grayDiff.draw(0,0);
    
    ofPopStyle();
    
    
    m_trackingFbo.end();
    ofDisableAlphaBlending();
    
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
