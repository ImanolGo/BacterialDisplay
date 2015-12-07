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
    
    this->setupFluidGui();
    this->setupCameraGui();
    
}
void GuiManager::setupFluidGui()
{
    
    HaloVisuals& haloVisuals = AppManager::getInstance().getHaloManager()->getHaloVisuals();
    
    m_fluidGui.setup("FluidGUI", GUI_FLUID_SETTINGS_FILE_NAME);
    m_fluidGui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
    m_fluidGui.setDefaultFillColor(ofColor(160, 160, 160, 160));
    m_fluidGui.add(guiFPS.set("FPS", 0, 0, 60));
    
    int guiColorSwitch = 0;
    ofColor guiHeaderColor[2];
    guiHeaderColor[0].set(160, 160, 80, 200);
    guiHeaderColor[1].set(80, 160, 160, 200);
    ofColor guiFillColor[2];
    guiFillColor[0].set(160, 160, 80, 200);
    guiFillColor[1].set(80, 160, 160, 200);
    
    m_fluidGui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_fluidGui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_fluidGui.add(haloVisuals.m_opticalFlow.parameters);
    
    m_fluidGui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_fluidGui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_fluidGui.add(haloVisuals.m_velocityMask.parameters);
    
    m_fluidGui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_fluidGui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_fluidGui.add(haloVisuals.m_fluid.parameters);
    
    pictureCalibrationParameters.setName("calibration");
    pictureCalibrationParameters.add(offsetX.set("Offset X", 0, -50, 50));
    offsetX.addListener(&haloVisuals, &HaloVisuals::setOffsetX);
    pictureCalibrationParameters.add(offsetY.set("Offset Y", 0, -50, 50));
    offsetY.addListener(&haloVisuals, &HaloVisuals::setOffsetY);
    pictureCalibrationParameters.add(scaleX.set("Scale X", 1, 0, 2));
    scaleX.addListener(&haloVisuals, &HaloVisuals::setScaleX);
    pictureCalibrationParameters.add(scaleY.set("Scale Y", 1, 0, 2));
    scaleY.addListener(&haloVisuals, &HaloVisuals::setScaleY);
    
    m_fluidGui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_fluidGui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_fluidGui.add(pictureCalibrationParameters);
    
    visualisationParameters.setName("visualisation");
    visualisationParameters.add(showScalar.set("show scalar", true));
    visualisationParameters.add(showField.set("show field", true));
    visualisationParameters.add(displayScalarScale.set("display scalar scale", 0.25, 0.05, 0.5));
    displayScalarScale.addListener(&haloVisuals, &HaloVisuals::setDisplayScalarScale);
    visualisationParameters.add(velocityFieldArrowScale.set("arrow scale", 0.6, 0.2, 1));
    velocityFieldArrowScale.addListener(&haloVisuals, &HaloVisuals::setVelocityFieldArrowScale);
    visualisationParameters.add(temperatureFieldBarScale.set("temperature scale", 0.25, 0.05, 0.5));
    temperatureFieldBarScale.addListener(&haloVisuals, &HaloVisuals::setTemperatureFieldBarScale);
    visualisationParameters.add(visualisationLineSmooth.set("line smooth", false));
    visualisationLineSmooth.addListener(&haloVisuals, &HaloVisuals::setVisualisationLineSmooth);
    
    m_fluidGui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_fluidGui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_fluidGui.add(visualisationParameters);
    
    leftButtonParameters.setName("mouse left button");
    for (int i=0; i<3; i++) {
        leftButtonParameters.add(haloVisuals.m_flexDrawForces[i].parameters);
    }
    m_fluidGui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    m_fluidGui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    m_fluidGui.add(leftButtonParameters);
    
    m_fluidGui.loadFromFile(GUI_FLUID_SETTINGS_FILE_NAME);
    m_fluidGui.minimizeAll();
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
