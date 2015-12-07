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

DisplayManager::DisplayManager(): Manager(), m_ringSize(0.0), m_showRingsPreview(true)
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
    
    m_DisplayFadeCandys.clear();
    this->setupDisplayUnits();
    this->setupOPC();
    
    m_DisplayVisuals.setup();
    
    ofLogNotice() <<"DisplayManager::initialized" ;
    
}


void DisplayManager::setupDisplayUnits()
{
    this->createDisplayUnitsPositions();
    this->createDisplayUnits();
}

void DisplayManager::createDisplayUnitsPositions()
{
    
    int maxRingsInRow = 5;
    int minRingsInRow = 4;
    float numRingsHeight = 10;
    float numMarginsHeight = numRingsHeight - 1;
    float numMarginsWidth = maxRingsInRow + minRingsInRow - 1;
    float marginRatio = 0.3;
    
    
    float layoutMargin = 20;
    float wallWidth = ofGetWidth()*0.5 - layoutMargin*2;
    m_ringPreviewSize = wallWidth/(maxRingsInRow + minRingsInRow + numMarginsWidth*marginRatio);
    
    //float wallHeight = ofGetHeight() - layoutMargin*4;
    //m_ringPreviewSize = wallHeight/(numRingsHeight + marginRatio*numMarginsHeight);
    m_ringSize = 30;
    float scale = m_ringSize/m_ringPreviewSize;
    float margin = m_ringPreviewSize*marginRatio;
    float wallHeight = m_ringPreviewSize*(numRingsHeight) + numMarginsHeight*margin;
    
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
 
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  preview size = " << m_ringPreviewSize;
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  ring size = " << m_ringSize;
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  m_previewRectangle:  x = " << m_previewRectangle.x << ", y = " << m_previewRectangle.y << ", w = " << m_previewRectangle.width << ", h = " << m_previewRectangle.height;
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  m_imageSpaceRectangle:  x = " << m_imageSpaceRectangle.x << ", y = " << m_imageSpaceRectangle.y << ", w = " << m_imageSpaceRectangle.height << ", h = " << m_imageSpaceRectangle.width;
    
    RingSettingsVector ringsSettingsVector = AppManager::getInstance().getSettingsManager()->getRingsSettingsVector();
    
    int colInd = 0;
    int rowInd = 0;
    int colMax = minRingsInRow;
    float leftMargin = m_ringPreviewSize + margin;
    
    ofLogNotice() <<"DisplayManager::createDisplayUnitsPositions->  ringsSettingsVector.size() = " << ringsSettingsVector.size();
    for (int i = 1; i < ringsSettingsVector.size()+1; i++) {
        
        if(colInd>=colMax){
            colInd=0;
            rowInd++;
            
            if((rowInd%2==0)){
                colMax = minRingsInRow;
                leftMargin = m_ringPreviewSize + margin;
            }
            else{
                colMax = maxRingsInRow;
                leftMargin = 0;
            }
        }
        
        float x = m_previewRectangle.x + leftMargin + m_ringPreviewSize*0.5 + (2*m_ringPreviewSize + 2*margin)*colInd;
        float y = m_previewRectangle.y + m_ringPreviewSize*0.5 + (m_ringPreviewSize*0.5 + margin*0.5)*rowInd;
        
        m_DisplayUnitsPreviewPositionMap[i] = ofVec3f(x,y,0);
       
        
        //ofLogNotice() <<"DisplayManager::createDisplayUnitsPreviewPositions->  i = " << i << ", x = " << x << ", y = " << y;
        
       
        x = (x - m_previewRectangle.x)*scale + m_imageSpaceRectangle.x;
        y = (y - m_previewRectangle.y)*scale + m_imageSpaceRectangle.y;
        
        m_DisplayUnitsPositionMap[i] = ofVec3f(x,y,0);
        
        
        colInd++;
    }
    
    
}

void DisplayManager::createDisplayUnits()
{
    RingSettingsVector ringsSettingsVector = AppManager::getInstance().getSettingsManager()->getRingsSettingsVector();
    
    for (int i = 0; i < ringsSettingsVector.size(); i++) {
        DisplayUnitSettings settings = ringsSettingsVector[i];
        ofPtr<DisplayUnit> DisplayUnit= this->createSingleDisplayUnit(settings);
        this->addDisplayUnit(DisplayUnit);
        
    }
}

ofPtr<DisplayUnit> DisplayManager::createSingleDisplayUnit(const DisplayUnitSettings& settings)
{
    ofVec3f ringPosition;
    ofVec3f ringPreviewPosition;
    int key = settings.positionIndex;
    if (m_DisplayUnitsPositionMap.find(key)!= m_DisplayUnitsPositionMap.end()) {
        ringPosition = m_DisplayUnitsPositionMap[key];
        ringPreviewPosition = m_DisplayUnitsPreviewPositionMap[key];
    }
    
    //ofLogNotice() <<"DisplayManager::createDisplayUnits->  id = " << settings.id  <<", channel = " << settings.channel
    //<<", fadeCandyInd = "<< settings.fadeCandyInd << ", numberLeds = " <<  settings.numberLeds <<
    // ", x = " <<  ringPosition.x <<  ", y = " <<  ringPosition.y ;
    
    BasicVisual basicVisual = BasicVisual(ringPosition, m_ringSize, m_ringSize);
    ofPtr<DisplayUnit> DisplayUnit = ofPtr<DisplayUnit>(new DisplayUnit(basicVisual,settings));
    
    
    basicVisual = BasicVisual(ringPreviewPosition, m_ringPreviewSize, m_ringPreviewSize);
    DisplayUnit->setDisplayUnitPreview(basicVisual);
    
    return DisplayUnit;
}

void DisplayManager::addDisplayUnit(ofPtr<DisplayUnit> DisplayUnit)
{
    int fadeCandyId = DisplayUnit->getFadeCandyNum();
    
    if(m_DisplayFadeCandys.find(fadeCandyId)==m_DisplayFadeCandys.end()){
         ofPtr<DisplayFadeCandy> DisplayFadeCandy = ofPtr<DisplayFadeCandy>(new DisplayFadeCandy(fadeCandyId));
         m_DisplayFadeCandys[fadeCandyId] = DisplayFadeCandy;
    }
   
    m_DisplayFadeCandys[fadeCandyId]->addDisplayUnit(DisplayUnit);
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
    m_DisplayVisuals.update();
}


void DisplayManager::grabImageData()
{
    m_screenPixels.clear();
    m_screenImage.clear();
    m_screenImage.grabScreen(m_imageSpaceRectangle.x,m_imageSpaceRectangle.y,m_imageSpaceRectangle.width,m_imageSpaceRectangle.height);
    m_screenPixels = m_screenImage.getPixelsRef(); // Transfer grab data to the pixel array
}

void DisplayManager::updateFadeCandys()
{
    // If the client is not connected do not try and send information
    if (!m_opcClient.isConnected()){
        // Will continue to try and reconnect to the Pixel Server
        m_opcClient.tryConnecting();
        return;
    }
  
    for(DisplayFadeCandyMap::iterator it = m_DisplayFadeCandys.begin(); it != m_DisplayFadeCandys.end(); it++){
        it->second->updateDisplayUnits(m_imageSpaceRectangle, m_screenPixels);
        m_opcClient.writeFadeCandy( it->second->getId(), it->second->getColorData());
    }
    
}

void DisplayManager::draw()
{
    m_DisplayVisuals.draw();
    
    if (m_showRingsPreview) {
        this->drawRectangles();
        this->drawDisplayFadeCandys();
    }
   
}

void DisplayManager::drawDisplayFadeCandys()
{
    for(DisplayFadeCandyMap::iterator it = m_DisplayFadeCandys.begin(); it != m_DisplayFadeCandys.end(); it++){
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
    ofRect(m_imageSpaceRectangle);
    
    ofFill();
    ofSetColor(150);
    ofRect(m_previewRectangle);
    
    ofPopStyle();
    ofPopMatrix();

}

