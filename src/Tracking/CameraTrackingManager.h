/*
 *  CameraTrackingManager.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#pragma once

#include "Manager.h"

#if defined( TARGET_LINUX_ARM )
    #include "ofxRPiCameraVideoGrabber.h"
    #include "ImageFilterCollection.h"
#endif

#include "ofxOpenCv.h"

//========================== class CameraTrackingManager ==============================
//============================================================================
/** \class CameraTrackingManager CameraTrackingManager.h
 *	\brief Class managing the camera tracking
 *	\details It creates camera tracker and sends tracking information
 */

class CameraTrackingManager: public Manager
{
    
    static const int CAMERA_WIDTH;
    static const int CAMERA_HEIGHT;
    
public:
    
    //! Constructor
    CameraTrackingManager();
    
    //! Destructor
    ~CameraTrackingManager();
    
    //! Set-up the camera tracking
    void setup();
    
    //! Update the camera tracking
    void update();
    
    //! Draw camera tracking
    void draw();
    
    void showCamera(bool show){m_showCamera = show;}
    
    const int getWidth(){return m_videoGrabber.getWidth();}
    
    const int getHeight(){return m_videoGrabber.getHeight();}
    
    const bool isNewFrame(){return m_videoGrabber.isFrameNew();}
    
    const ofFbo& getCameraFbo(){return m_cameraFbo;}
    
    const ofFbo& getTrackedImage(){return m_trackingFbo;}
    
    void onHueChange(float & value);
    
    void onHueAlphaChange(float & value);
    
    void onHueChangeRate(float & value);
    
   
    
private:
    
    //! Set-up the camera tracking
    void setupCamera();
    
    void setupOpenCv();
    
    void updateCamera();
    
    void updateHue();
    
    void updateOpenCv();
    
    void drawCamera();
    
    void drawTracking();
    
    void drawROI();
    
    void drawGrayDiff();
    
    void drawHueColor();
    
private:
    
    
    ofVideoGrabber      m_videoGrabber;
    
    float               m_hueChangeRate; //It tells you how many seconds do you need to have a unit hue change
    ofColor             m_hueColor;
    ofRectangle         m_cameraArea;
    
    ofFbo				m_cameraFbo;
    ofFbo				m_trackingFbo;
    
    bool                m_showCamera;
    
    #if defined( TARGET_LINUX_ARM )
        ofxRPiCameraVideoGrabber m_videoGrabberPi;
        OMXCameraSettings m_omxCameraSettings;
        ImageFilterCollection m_filterCollection;
    #endif
    
    
    ofxCvColorImage         m_colorImg;
    ofxCvGrayscaleImage 	m_grayImage;
    ofxCvGrayscaleImage 	m_grayBg;
    ofxCvGrayscaleImage 	m_grayDiff;
    
    ofxCvHaarFinder         m_finder;
    
    ofRectangle             m_roi;
    ofPixels                m_pixels;
    
};

//==========================================================================


