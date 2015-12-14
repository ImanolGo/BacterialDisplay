//
//  DisplayUnit.cpp
//
//  Created by Imanol Gomez on 01/03/15.
//
//--------------------------------------------------------------

#include "DisplayUnit.h"


DisplayUnitPreview::DisplayUnitPreview(const BasicVisual& visual, const DisplayUnitSettings& settings)
{
    m_position = visual.getPosition();
    m_width = visual.getWidth();
    m_height = visual.getHeight();
    m_settings = settings;
    
    this->setup();
}

DisplayUnitPreview::~DisplayUnitPreview()
{
    //Intentionaly left empty
}

void DisplayUnitPreview::setup()
{
    this->setupTextVisual();
    this->setupImageVisuals();
}

void DisplayUnitPreview::setupTextVisual()
{
    string fontPath = "fonts/helvetica-neue-medium.ttf";
    float fontSize = m_width*0.25;
    
    m_textVisual = ofPtr<TextVisual>(new TextVisual(m_position,m_width,m_height,true));
    m_textVisual->setText(m_settings.id,fontPath,fontSize,ofColor::black);
}


void DisplayUnitPreview::setupImageVisuals()
{
    m_ringVisual.setResource("Ring");
    m_ringVisual.setWidth(m_width);
    m_ringVisual.setHeight(m_height);
    m_ringVisual.setCentred(true);
    m_ringVisual.setPosition(m_position);
    
    
    float pixelSize = 10;
    m_ledVisual.setResource("Pixel");
    m_ledVisual.setWidth(pixelSize);
    m_ledVisual.setHeight(pixelSize);
    m_ledVisual.setCentred(true);
}

void DisplayUnitPreview::draw()
{
    this->drawUnit();
    this->drawID();
}

void DisplayUnitPreview::drawUnit()
{

    
    ofSetCircleResolution(10);
    
    float pixelRadius = m_ledVisual.getWidth()*0.5;
    float margin = pixelRadius*2;
    
    
    ofPushMatrix();
    ofPushStyle();
    ofEnableAlphaBlending();

    
    if(m_settings.channel%2==0)
    {
        m_ringVisual.setColor(ofColor::black);
    }
    else{
        m_ringVisual.setColor(ofColor::white);
    }
    
    m_ringVisual.draw();
    //ofDrawEllipse(m_position.x , m_position.y, m_width, m_height);
    //ofEllipse(m_position.x , m_position.y, m_width-margin*2, m_height-margin*2);
    
    float angleStep = (2.0 * M_PI)/m_settings.numberLeds;
    float offsetAngle = 0;
    
    ofFill();
    for (int i = 0; i < m_settings.numberLeds; i++)
    {
        float angle =  i * angleStep - offsetAngle;
        angle = 2*M_PI - angle; // Inverse the angle
        float rx = m_position.x  + 0.5 * (m_width - margin) * cos(angle);
        float ry = m_position.y + 0.5 * (m_height - margin) * sin(angle);
        
        ofColor color(m_ledColor[i]);
        m_ledVisual.setColor(color);
        m_ledVisual.setPosition(ofPoint(rx,ry));
        m_ledVisual.draw();
        
        //ofSetColor(m_ledColor[i]);
        //ofDrawCircle(rx,ry,pixelRadius);
    }
    
    ofDisableAlphaBlending();
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void DisplayUnitPreview::drawID(bool hideText)
{
    if (hideText) {
        return;
    }
    
    ofEnableAlphaBlending();
    ofSetColor(255);
    m_textVisual->draw();
    ofDisableAlphaBlending();
}


void DisplayUnit::setDisplayUnitPreview(const BasicVisual& visual)
{
    m_displayUnitPreview = ofPtr<DisplayUnitPreview>(new DisplayUnitPreview(visual,m_settings));
}

DisplayUnit::DisplayUnit(const BasicVisual& visual, const DisplayUnitSettings& settings): BasicVisual(), m_margin(0.0)
{
    m_position = visual.getPosition();
    m_width = visual.getWidth();
    m_height = visual.getHeight();
    m_settings = settings;
    
    m_displayUnitPreview = ofPtr<DisplayUnitPreview>(new DisplayUnitPreview(visual,m_settings));
    
    this->setup();
}

DisplayUnit::~DisplayUnit()
{
    //Intentionaly left empty
}


void DisplayUnit::setup()
{
    this->setupDisplayUnit();
    this->setupTextVisual();
}

void DisplayUnit::setupDisplayUnit()
{
    
    m_ledColor = ofColor::black;
    
    float angleStep = (2.0 * M_PI)/m_settings.numberLeds;
    float offsetAngle = 0;
    m_margin = 10;
    
    for (int i = 0; i < m_settings.numberLeds; i++)
    {
        float angle =  i * angleStep - offsetAngle;
        angle = 2*M_PI - angle; // Inverse the angle
        float rx = m_position.x  + 0.5 * (m_width - m_margin) * cos(angle);
        float ry = m_position.y + 0.5 * (m_height - m_margin) * sin(angle);
        
        m_ledPositions.push_back(ofVec2f(rx,ry));
    }

}

void DisplayUnit::setupTextVisual()
{
    string fontPath = "fonts/helvetica-neue-medium.ttf";
    float fontSize = m_width*0.25;
    string stringId = ofToString(m_settings.id);
    
    m_textVisual = ofPtr<TextVisual>(new TextVisual(m_position,m_width,m_height,true));
    m_textVisual->setText(stringId,fontPath,fontSize,ofColor::white);
}


//--------------------------------------------------------------

void DisplayUnit::setPixels(const ofRectangle& grabArea, const ofPixels& screenPixels)
{
    for (int i = 0; i < m_ledPositions.size(); i++)
    {
        float x = m_ledPositions[i].x  - grabArea.x;
        float y = m_ledPositions[i].y  - grabArea.y;
        m_ledColor[i] = screenPixels.getColor(x, y).getBrightness();
        //m_ledColor = screenPixels.getColor(x, y);
    }
    
    m_displayUnitPreview->setColor(m_ledColor);
}

//--------------------------------------------------------------
void DisplayUnit::drawGrabRegion(bool hideArea)
{
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);

    if (hideArea == true){
        // Draw Interaction Area
        ofSetColor(255, 255);
    }
    else{
        // Visualise the Grabber
        ofSetColor(0, 175);
    }
    
    ofDrawEllipse(m_position.x, m_position.y, m_width, m_height);
    //ofDrawEllipse(m_position.x, m_position.y, m_width-m_margin*2, m_height-m_margin*2);
    
    float angleStep = (2.0 * M_PI)/m_settings.numberLeds;
    float offsetAngle = 0;
    
    
    ofSetLineWidth(1);
    for (int i = 0; i < m_ledPositions.size(); i++)
    {
        ofDrawCircle(m_ledPositions[i],2);
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void DisplayUnit::drawID(bool hideText)
{
    if (hideText) {
        return;
    }
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(0, 175);
    m_textVisual->draw();
    ofDisableAlphaBlending();
    ofPopStyle();
}

void DisplayUnit::draw()
{
    m_displayUnitPreview->draw();
    this->drawGrabRegion();
    //this->drawID();
}