/*
 *  GuiManager.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"

const string GuiManager::GUI_CAMERA_SETTINGS_FILE_NAME = "xmls/CameraGuiSettings.xml";

GuiManager::GuiManager(): Manager(), m_showGui(true)
{
	//Intentionally left empty
}


GuiManager::~GuiManager()
{
    this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
	if(m_initialized)
		return;

    ofLogNotice() <<"GuiManager::initialized";

	Manager::setup();

    this->setupCameraGui();
}
    
void GuiManager::setupCameraGui()
{
    
    m_cameraGui.setup("CameraGUI", GUI_CAMERA_SETTINGS_FILE_NAME);
    //m_cameraGui.loadFont(OF_TTF_MONO,16,true,true);
    m_cameraGui.setPosition(20,40);
    //m_cameraGui.setWidthElements(100);
    //m_cameraGui.setDefaultHeight(40);
    //m_cameraGui.setDefaultWidth(500);
    
    ofPtr<CameraTrackingManager> cameraTrackingManager = AppManager::getInstance().getCameraTrackingManager();
    ofPtr<DisplayManager> displayManager = AppManager::getInstance().getDisplayManager();
   
    ofxFloatSlider * hue = new ofxFloatSlider();
    hue->setup("Hue", 0.5, 0.0, 1.0);
    hue->addListener(cameraTrackingManager.get(), &CameraTrackingManager::onHueChange);
   // m_cameraGui.add(hue);
    
    ofxFloatSlider * hueAlpha = new ofxFloatSlider();
    hueAlpha->setup("Hue Alpha", 0.5, 0.0, 1.0);
    hueAlpha->addListener(cameraTrackingManager.get(), &CameraTrackingManager::onHueAlphaChange);
    //m_cameraGui.add(hueAlpha);
    
    ofxFloatSlider * hueChangeRate = new ofxFloatSlider();
    hueChangeRate->setup("Hue Change Rate (s)", 1.0, 0.01, 10.0);
    hueChangeRate->addListener(cameraTrackingManager.get(), &CameraTrackingManager::onHueChangeRate);
    //m_cameraGui.add(hueChangeRate);
    
    m_flickering.set("Flickering", 0.0, 0.0, 1.0);
    m_flickering.addListener(displayManager.get(), &DisplayManager::onFlickeringCHange);
    m_cameraGui.add(m_flickering);
    
    m_numberPixels.set("NumberPixels", 16, 8, 32);
    m_numberPixels.addListener(cameraTrackingManager.get(), &CameraTrackingManager::onNumberPixelsChange);
    m_cameraGui.add(m_numberPixels);
    
    
    m_contrastCamera.set("Contrast", ofVec4f(0), ofVec4f(0), ofVec4f(1));
    m_contrastCamera.addListener(cameraTrackingManager.get(), &CameraTrackingManager::onContrastChange);
    m_cameraGui.add(m_contrastCamera);

    
    m_cameraGui.loadFromFile(GUI_CAMERA_SETTINGS_FILE_NAME);
    
    
}

void GuiManager::setHue(float hue)
{
    ofxFloatSlider & hueSlider = m_cameraGui.getFloatSlider("Hue");
    hueSlider = hue;
    
}

void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    m_cameraGui.draw();
    guiFPS = ofGetFrameRate();
    
}


void GuiManager::saveGuiValues()
{
    m_cameraGui.saveToFile(GUI_CAMERA_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
     m_cameraGui.loadFromFile(GUI_CAMERA_SETTINGS_FILE_NAME);
}

void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}


void GuiManager::increaseFlickering()   
{
    m_flickering += 0.05;
    m_flickering = ofClamp(m_flickering, 0, 1);
}

void GuiManager::decreaseFlickering()
{
    m_flickering -= 0.05;
    m_flickering = ofClamp(m_flickering, 0, 1);
}

