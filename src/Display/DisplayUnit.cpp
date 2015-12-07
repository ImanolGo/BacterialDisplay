//
//  HaloRing.cpp
//
//  Created by Imanol Gomez on 01/03/15.
//
//--------------------------------------------------------------

#include "HaloRing.h"


HaloRingPreview::HaloRingPreview(const BasicVisual& visual, string id)
{
    m_position = visual.getPosition();
    m_width = visual.getWidth();
    m_height = visual.getHeight();
    m_id = id;
    
    this->setup();
}

HaloRingPreview::~HaloRingPreview()
{
    //Intentionaly left empty
}

void HaloRingPreview::setup()
{
    this->setupTextVisual();
}

void HaloRingPreview::setupTextVisual()
{
    string fontPath = "fonts/helvetica-neue-medium.ttf";
    float fontSize = m_width*0.25;
    
    m_textVisual = ofPtr<TextVisual>(new TextVisual(m_position,m_width,m_height,true));
    m_textVisual->setText(m_id,fontPath,fontSize,ofColor::black);
}

void HaloRingPreview::draw()
{
    this->drawLedRing();
    this->drawID();
}

void HaloRingPreview::drawLedRing()
{
    
    ofSetCircleResolution(10);
    
    float pixelSize = 1;
    float margin = pixelSize*2;
    
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(0);
    //ofEllipse(m_position.x , m_position.y, m_width, m_height);
    //ofEllipse(m_position.x , m_position.y, m_width-margin*2, m_height-margin*2);
    
    float angleStep = (2.0 * M_PI)/m_ledColors.size();
    float offsetAngle = M_PI*0.5 - angleStep*0.5;
    
    ofFill();
    for (int i = 0; i < m_ledColors.size(); i++)
    {
        float angle =  i * angleStep - offsetAngle;
        angle = 2*M_PI - angle; // Inverse the angle
        float rx = m_position.x  + 0.5 * (m_width - margin) * cos(angle);
        float ry = m_position.y + 0.5 * (m_height - margin) * sin(angle);
        
        ofSetColor(m_ledColors[i]);
        ofCircle(rx,ry,pixelSize);
    }
    
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void HaloRingPreview::drawID(bool hideText)
{
    if (hideText) {
        return;
    }
    
    ofSetColor(255);
    m_textVisual->draw();
}


void HaloRing::setHaloRingPreview(const BasicVisual& visual)
{
    m_haloRingPreview = ofPtr<HaloRingPreview>(new HaloRingPreview(visual,m_settings.id));
}

HaloRing::HaloRing(const BasicVisual& visual, const HaloRingSettings& settings): BasicVisual(), m_margin(0.0)
{
    m_position = visual.getPosition();
    m_width = visual.getWidth();
    m_height = visual.getHeight();
    m_settings = settings;
    
    m_haloRingPreview = ofPtr<HaloRingPreview>(new HaloRingPreview(visual,m_settings.id));
    
    this->setup();
}

HaloRing::~HaloRing()
{
    //Intentionaly left empty
}


void HaloRing::setup()
{
    this->setupLedRing();
    this->setupTextVisual();
}

void HaloRing::setupLedRing()
{
    // Set the pixel data
    //m_screenPixels.allocate(m_width, m_height,GL_RGB);
    
    float angleStep = (2.0 * M_PI)/m_settings.numberLeds;
    float offsetAngle = M_PI*0.5 - angleStep*0.5;
    m_margin = 10;
    
    for (int i = 0; i < m_settings.numberLeds; i++)
    {
        float angle =  i * angleStep - offsetAngle;
        angle = 2*M_PI - angle; // Inverse the angle
        
        // Generate the position of the grabber points
        float rx = m_position.x  + 0.5 * (m_width-m_margin) * cos(angle);
        float ry = m_position.y + 0.5 * (m_height-m_margin) * sin(angle);
        m_ledPositions.push_back(ofVec2f(rx,ry));
        m_ledColors.push_back(ofColor::black);
    }
}

void HaloRing::setupTextVisual()
{
    string fontPath = "fonts/helvetica-neue-medium.ttf";
    float fontSize = m_width*0.25;
    string stringId = ofToString(m_settings.id);
    
    m_textVisual = ofPtr<TextVisual>(new TextVisual(m_position,m_width,m_height,true));
    m_textVisual->setText(stringId,fontPath,fontSize,ofColor::white);
}

//--------------------------------------------------------------

void HaloRing::setPixels(const ofRectangle& grabArea, const ofPixels& screenPixels)
{
    for (int i = 0; i < m_ledPositions.size(); i++)
    {
        float x = m_ledPositions[i].x  - grabArea.x;
        float y = m_ledPositions[i].y  - grabArea.y;
        m_ledColors[i] = screenPixels.getColor(x, y);
    }
    
    m_haloRingPreview->setColors(m_ledColors);
}

//--------------------------------------------------------------
void HaloRing::drawGrabRegion(bool hideArea)
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
    
    ofEllipse(m_position.x, m_position.y, m_width, m_height);
    ofEllipse(m_position.x, m_position.y, m_width-m_margin*2, m_height-m_margin*2);
    
    ofSetLineWidth(1);
    for (int i = 0; i < m_ledPositions.size(); i++)
    {
        ofCircle(m_ledPositions[i],2);
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void HaloRing::drawID(bool hideText)
{
    if (hideText) {
        return;
    }
    
    ofSetColor(0, 175);
    m_textVisual->draw();
}

void HaloRing::draw()
{
    m_haloRingPreview->draw();
    //this->drawGrabRegion();
    this->drawID();
}