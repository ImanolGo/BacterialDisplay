/*
 *  DisplayVisuals.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */


#pragma once

#include "ImageVisual.h"

#include "ofxGui.h"


//========================== class DisplayVisuals ==============================
//============================================================================
/** \class DisplayVisuals DisplayVisuals.h
 *	\brief Class managing the display visuals
 *	\details it creates and updates the display visuals
 */


class DisplayVisuals {
    
public:
    
    //! Constructor
    DisplayVisuals();
    
    //! Destructor
    ~DisplayVisuals();
    
    //! Setup the Halo Visuals
    void setup();
    
    //! Update the Halo Visuals
    void update();
    
    //! Draw the Halo Visuals
    void draw();
    
    //! Set the mode of the Halo Visuals
    void setMode(int mode) {m_mode = mode;}
  
    void setOffsetX(float & dx);
    void setOffsetY(float & dy);
    void setScaleX(float & sx);
    void setScaleY(float & sy);
    
    
private:
    
    void createImageVisuals();
    
    void drawVisuals();
    
    void setupDisplayArea();
    
    void drawCircle();
    
    void drawParticle();
    
    void drawCamera();
    
    void calibrateDisplayArea();
    
    
private:
    
    int m_mode;
    ofPtr<ImageVisual>  m_imageVisual;
    
    ofVec2f				m_lastMouse;
    
    ofRectangle         m_displayArea;
    ofRectangle         m_calibratedDisplayArea;
    ofPoint             m_displayOffset;
    ofPoint             m_displayScale;
    
};



