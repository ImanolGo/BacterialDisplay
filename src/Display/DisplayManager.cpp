/*
 *  DisplayManager.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */


#include "ofMain.h"

#include "DisplayManager.h"
#include "AppManager.h"
#include "SettingsManager.h"


const int DisplayManager::NUM_FADE_CANDYS = 11;
const int DisplayManager::NUM_HALO_RINGS = 85;
const int DisplayManager::NUM_HALO_LEDS = 48;

DisplayManager::DisplayManager(): Manager(), m_displayUnitSize(0.0), m_showDisplayPreview(true)
{
	//Intentionally left empty
}


DisplayManager::~DisplayManager()
{
    ofLogNotice() <<"DisplayManager::Destructor" ;
}


void DisplayManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();
    
    m_displayFadeCandys.clear();
    this->setupDisplayUnits();
    this->setupOPC();
    
    m_displayVisuals.setup();
    
    ofLogNotice() <<"DisplayManager::initialized" ;
    
}


void DisplayManager::setupDisplayUnits()
{
    this->createDisplayUnitSettings();
    this->createDisplayUnits();
}


void DisplayManager::createDisplayUnitSettings()
{
    int maxUnitsInRow = 14;
    int minUnitsInRow = maxUnitsInRow-1;
    int maxNumRows = 19;
    
    float marginRatio = 0.3;
    
    float layoutMargin = 20;
    float wallWidth = ofGetWidth()*0.5 - layoutMargin*2;
    m_displayUnitPreviewSize = wallWidth/(maxUnitsInRow + (maxUnitsInRow-1)*marginRatio);
    
    
    m_displayUnitSize = 25;
    float scale = m_displayUnitSize/m_displayUnitPreviewSize;
    float margin = m_displayUnitPreviewSize*marginRatio;
    float wallHeight = m_displayUnitPreviewSize*(maxNumRows);
    
    float w = wallWidth;
    float h = wallHeight;
    float x = ofGetWidth()*0.25 - w*0.5;
    float y = ofGetHeight()*0.5 - h*0.5;
    
    m_previewRectangle = ofRectangle(x,y,w,h);
    
    
    w = wallWidth*scale;
    h = wallHeight*scale;
    x = ofGetWidth()*0.75 - w*0.5;
    y = ofGetHeight()*0.75 - h*0.5;
    
    m_imageSpaceRectangle = ofRectangle(x,y,w,h);
    
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  preview size = " << m_displayUnitPreviewSize;
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  unit size = " << m_displayUnitSize;
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  m_previewRectangle:  x = " << m_previewRectangle.x << ", y = " << m_previewRectangle.y << ", w = " << m_previewRectangle.width << ", h = " << m_previewRectangle.height;
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  m_imageSpaceRectangle:  x = " << m_imageSpaceRectangle.x << ", y = " << m_imageSpaceRectangle.y << ", w = " << m_imageSpaceRectangle.height << ", h = " << m_imageSpaceRectangle.width;
    
    

    
    int maxUnitsInCol = 10;
    int minUnitsInCol = maxUnitsInCol-1;
    int rowMax = maxUnitsInCol;
    int rowInd = rowMax - 1;
    int colInd = 0;
 
    
    
    int totalUnits = maxUnitsInRow*maxUnitsInCol + minUnitsInRow*minUnitsInCol;
    float topMargin = 0;
    
    int pixelsPerChannel = 38;
    int channel = 1;
    int index = 0;
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  total units= " << totalUnits;
    for (int i = 0; i < totalUnits; i++) {
        
        if(rowInd>=rowMax || rowInd<0){
            
            colInd++;
            
            if((colInd%2==0)){
                rowMax = maxUnitsInCol;
                rowInd = rowMax - 1;
                topMargin = 0;
            }
            else{
                rowMax = minUnitsInCol;
                rowInd=0;
                topMargin = m_displayUnitPreviewSize;
            }
        }
        
        channel = i/pixelsPerChannel + 1;
        index = i % pixelsPerChannel;
        
        DisplayUnitSettings settings; settings.id = ofToString(i+1); settings.channel = channel; settings.index = index; settings.numberLeds = 2;
        m_displayUnitsSettings[i] = settings;

        ofLogNotice() <<"DisplayManager::createDisplayUnitsPreviewPositions->  channel = " << channel << ", index = " << index ;
        
        float x = m_previewRectangle.x + m_displayUnitPreviewSize*0.5 + (m_displayUnitPreviewSize + margin)*colInd*0.5;
        float y = m_previewRectangle.y + topMargin + m_displayUnitPreviewSize*0.5 + (2*m_displayUnitPreviewSize)*rowInd;
        
        m_displayUnitsPreviewPositionMap[i] = ofVec3f(x,y,0);
        
        
        //ofLogNotice() <<"DisplayManager::createDisplayUnitsPreviewPositions->  i = " << i << ", x = " << x << ", y = " << y;
        
        
        x = (x - m_previewRectangle.x)*scale + m_imageSpaceRectangle.x;
        y = (y - m_previewRectangle.y)*scale + m_imageSpaceRectangle.y;
        
        m_displayUnitsPositionMap[i] = ofVec3f(x,y,0);
        
        
        if((colInd%2==0)){
            rowInd--;
        }
        else{
            rowInd++;
        }
    }
    
    
}

void DisplayManager::createDisplayUnits()
{
    
    for (auto settings : m_displayUnitsSettings) {
        ofPtr<DisplayUnit> DisplayUnit= this->createSingleDisplayUnit(settings.first);
        this->addDisplayUnit(DisplayUnit);
    }
}

ofPtr<DisplayUnit> DisplayManager::createSingleDisplayUnit(int id)
{
    ofVec3f displayUnitPosition;
    ofVec3f displayUnitPreviewPosition;
    DisplayUnitSettings settings;

    if (m_displayUnitsSettings.find(id)!= m_displayUnitsSettings.end()) {
        displayUnitPosition = m_displayUnitsPositionMap[id];
        displayUnitPreviewPosition = m_displayUnitsPreviewPositionMap[id];
        settings = m_displayUnitsSettings[id];
    }
    
    ofLogNotice() <<"DisplayManager::createDisplayUnits->  id = " << settings.id  <<", channel = " << settings.channel
    <<", index = "<< settings.index << ", numberLeds = " <<  settings.numberLeds <<
     ", x = " <<  displayUnitPosition.x <<  ", y = " <<  displayUnitPosition.y ;
    
    BasicVisual basicVisual = BasicVisual(displayUnitPosition, m_displayUnitSize, m_displayUnitSize);
    ofPtr<DisplayUnit> displayUnit = ofPtr<DisplayUnit>(new DisplayUnit(basicVisual,settings));
    
    
    basicVisual = BasicVisual(displayUnitPreviewPosition, m_displayUnitPreviewSize, m_displayUnitPreviewSize);
    displayUnit->setDisplayUnitPreview(basicVisual);
    
    return displayUnit;
}

void DisplayManager::addDisplayUnit(ofPtr<DisplayUnit> DisplayUnit)
{
    int fadeCandyId = DisplayUnit->getFadeCandyNum();
    
    if(m_displayFadeCandys.find(fadeCandyId)==m_displayFadeCandys.end()){
         ofPtr<DisplayFadeCandy> displayFadeCandy = ofPtr<DisplayFadeCandy>(new DisplayFadeCandy(fadeCandyId));
         m_displayFadeCandys[fadeCandyId] = displayFadeCandy;
    }
   
    m_displayFadeCandys[fadeCandyId]->addDisplayUnit(DisplayUnit);
}

void DisplayManager::setupOPC()
{
    string ipAddress = AppManager::getInstance().getSettingsManager()->getIpAddress();
    int port = AppManager::getInstance().getSettingsManager()->getPort();
    
    // Connect to the fcserver
    m_opcClient.setup(ipAddress, port, NUM_FADE_CANDYS);
    
    ofLogNotice() <<"DisplayManager::setupOPC -> Connect OPC Client to IP Address: " << ipAddress << ", port: " << port;
    
}

void DisplayManager::update()
{
    this->grabImageData();
    this->updateFadeCandys();
    m_displayVisuals.update();
}


void DisplayManager::grabImageData()
{
    m_screenPixels.clear();
    m_screenImage.clear();
    m_screenImage.grabScreen(m_imageSpaceRectangle.x,m_imageSpaceRectangle.y,m_imageSpaceRectangle.width,m_imageSpaceRectangle.height);
    m_screenPixels = m_screenImage.getPixels(); // Transfer grab data to the pixel array
}

void DisplayManager::updateFadeCandys()
{
    // If the client is not connected do not try and send information
    if (!m_opcClient.isConnected()){
        // Will continue to try and reconnect to the Pixel Server
        m_opcClient.tryConnecting();
        return;
    }
  
    for(DisplayFadeCandyMap::iterator it = m_displayFadeCandys.begin(); it != m_displayFadeCandys.end(); it++){
        it->second->updateDisplayUnits(m_imageSpaceRectangle, m_screenPixels);
        m_opcClient.writeFadeCandy( it->second->getId(), it->second->getColorData());
    }
    
}

void DisplayManager::draw()
{
    m_displayVisuals.draw();
    
    if (m_showDisplayPreview) {
        this->drawRectangles();
        this->drawDisplayFadeCandys();
    }
   
}

void DisplayManager::drawDisplayFadeCandys()
{
    for(DisplayFadeCandyMap::iterator it = m_displayFadeCandys.begin(); it != m_displayFadeCandys.end(); it++){
        it->second->draw();
    }
}

void DisplayManager::drawRectangles()
{
    ofPushMatrix();
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(1);
    
    ofSetColor(255);
    ofDrawRectangle(m_imageSpaceRectangle);
    
    ofFill();
    ofSetColor(150);
    ofDrawRectangle(m_previewRectangle);
    
    ofPopStyle();
    ofPopMatrix();

}

