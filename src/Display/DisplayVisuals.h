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
#include "ofxFlowTools.h"

#define USE_FASTER_INTERNAL_FORMATS			// about 15% faster but gives errors from ofGLUtils
using namespace flowTools;

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
    
    //Gui
    void resetDrawForces(bool& _value) { if (_value) {for (int i=0; i<m_numDrawForces; i++) m_flexDrawForces[i].reset();}}
    void setDisplayScalarScale(float& _value) { m_displayScalar.setScale(_value); }
    void setVelocityFieldArrowScale(float& _value) { m_velocityField.setVectorSize(_value); }
    void setTemperatureFieldBarScale(float& _value) { m_temperatureField.setVectorSize(_value); }
    void setVisualisationLineSmooth(bool& _value) { m_velocityField.setLineSmooth(_value); }
    
    void setOffsetX(float & dx);
    void setOffsetY(float & dy);
    void setScaleX(float & sx);
    void setScaleY(float & sy);
    
    
private:
    
    void createImageVisuals();
    
    void drawVisuals();
    
    void setupFluid();
    
    void setupDisplayArea();
    
    void updateFluid();
    
    void drawFluid();
    
    void drawCircle();
    
    void drawParticle();
    
    void drawPaintFluid();
    
    void calibrateDisplayArea();
    
public:
    
    ftOpticalFlow		m_opticalFlow;
    ftVelocityMask		m_velocityMask;
    ftFluidSimulation	m_fluid;
    
    ftDisplayScalar		m_displayScalar;
    ftVelocityField		m_velocityField;
    ftTemperatureField	m_temperatureField;
    
    int					m_numDrawForces;
    ftDrawForce*		m_flexDrawForces;
    
private:
    
    int m_mode;
    ofPtr<ImageVisual>  m_imageVisual;
    
    ofVec2f				m_lastMouse;
    
    ofRectangle         m_displayArea;
    ofRectangle         m_calibratedDisplayArea;
    ofPoint             m_displayOffset;
    ofPoint             m_displayScale;
    
};



