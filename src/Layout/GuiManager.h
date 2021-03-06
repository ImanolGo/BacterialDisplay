/*
 *  GuiManager.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#pragma once

#include "Manager.h"
#include "ofxGui.h"
//========================== class GuiManager ==============================
//============================================================================
/** \class GuiManager GuiManager.h
 *	\brief Class managing the application´s grapical user interface (GUI)
 *	\details It creates the gui and the callback functions that will be affected
 *    by the gui
 */

class GuiManager: public Manager
{
    
    static const string GUI_CAMERA_SETTINGS_FILE_NAME;
    static const string GUI_FLUID_SETTINGS_FILE_NAME;
    
public:

    //! Constructor
    GuiManager();

    //! Destructor
    ~GuiManager();

    //! Set-up the gui
    void setup();
    
    //! Draw the gui
    void draw();
    
    void saveGuiValues();
    
    void loadGuiValues();
    
    void toggleGui();
    
    void showGui(bool show){m_showGui=show;}
    
    void setHue(float hue);
    
    void increaseFlickering();
    
    void decreaseFlickering();

private:

    void setupCameraGui();
    
private:
    
    // Fluid GUI
    ofxPanel			gui;
    
    ofParameter<float>	guiFPS;
    ofParameter<bool>	doFullScreen;
    void				setFullScreen(bool& _value) { ofSetFullscreen(_value);}
    ofParameter<bool>	toggleGuiDraw;
    ofParameter<bool>	doFlipCamera;
    ofParameter<int>	visualisationMode;
    ofParameter<string> visualisationName;
    int					numVisualisationModes;
    string				*visualisationModeTitles;

    ofParameter<ofVec4f>    m_contrastCamera;
    ofParameter<int>        m_numberPixels;
    
    ofParameterGroup	pictureCalibrationParameters;
    ofParameter<float>	offsetX;
    ofParameter<float>	offsetY;
    ofParameter<float>	scaleX;
    ofParameter<float>	scaleY;
    
    ofParameter<float>	m_flickering;

    // Camera GUI
    ofxPanel    m_cameraGui;      //Class creating a camera gui panel
    bool        m_showGui;  //It defines the whether the gui should be shown or not
};

//==========================================================================


