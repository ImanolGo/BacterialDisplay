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
const string GuiManager::GUI_FLUID_SETTINGS_FILE_NAME = "xmls/FluidGuiSettings.xml";


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
    
    ofRectangle fluidRectangle = m_fluidGui.getShape();
    
    m_cameraGui.setup("CameraGUI", GUI_CAMERA_SETTINGS_FILE_NAME);
    m_cameraGui.setPosition(fluidRectangle.getRight() + 20,fluidRectangle.y);
    
    ofPtr<CameraTrackingManager> cameraTrackingManager = AppManager::getInstance().getCameraTrackingManager();
   
    ofxFloatSlider * hue = new ofxFloatSlider();
    hue->setup("Hue", 0.5, 0.0, 1.0);
    hue->addListener(cameraTrackingManager.get(), &CameraTrackingManager::onHueChange);
    m_cameraGui.add(hue);
    
    ofxFloatSlider * hueAlpha = new ofxFloatSlider();
    hueAlpha->setup("Hue Alpha", 0.5, 0.0, 1.0);
    hueAlpha->addListener(cameraTrackingManager.get(), &CameraTrackingManager::onHueAlphaChange);
    m_cameraGui.add(hueAlpha);
    
    ofxFloatSlider * hueChangeRate = new ofxFloatSlider();
    hueChangeRate->setup("Hue Change Rate (s)", 1.0, 0.01, 10.0);
    hueChangeRate->addListener(cameraTrackingManager.get(), &CameraTrackingManager::onHueChangeRate);
    m_cameraGui.add(hueChangeRate);
    
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
    m_fluidGui.draw();
    
}


void GuiManager::saveGuiValues()
{
    m_cameraGui.saveToFile(GUI_CAMERA_SETTINGS_FILE_NAME);
    m_fluidGui.saveToFile(GUI_FLUID_SETTINGS_FILE_NAME);
}

void GuiManager::loadGuiValues()
{
     m_fluidGui.loadFromFile(GUI_FLUID_SETTINGS_FILE_NAME);
     m_cameraGui.loadFromFile(GUI_CAMERA_SETTINGS_FILE_NAME);
}

void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}
