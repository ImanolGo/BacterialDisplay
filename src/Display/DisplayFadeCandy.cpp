/*
 *  DisplayFadeCandy.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#include "ofxOPC.h"
#include "DisplayFadeCandy.h"


DisplayFadeCandy::DisplayFadeCandy(int id): m_id(id)
{
    this->setup();
}

DisplayFadeCandy::~DisplayFadeCandy()
{
    //Intentionaly left empty
}


void DisplayFadeCandy::setup()
{
    this->createLedColorVector();
}

void DisplayFadeCandy::createLedColorVector()
{
    m_ledColors.clear();
    
    size_t size = FADE_CANDY_NUM_CHANNELS*LEDS_PER_CHANNEL;
    m_ledColors = vector <ofColor> (size,ofColor::black);
}

void DisplayFadeCandy::addDisplayUnit(ofPtr<DisplayUnit> displayUnit)
{
    if (displayUnit!= NULL) {
        m_displayUnits[displayUnit->getId()] = displayUnit;
    }
    
}

void DisplayFadeCandy::updateDisplayUnits(const ofRectangle &grabArea, const ofPixels &screenPixels)
{
    for(DisplayUnitsMap::iterator it = m_displayUnits.begin(); it != m_displayUnits.end(); it++){
        
        int channel = it->second->getChannel();
        
        if (channel > 0 && channel <= FADE_CANDY_NUM_CHANNELS ) {
            int i = (channel-1)*LEDS_PER_CHANNEL + it->second->getIndex();
            
            it->second->setPixels(grabArea, screenPixels);
            m_ledColors[i] = it->second->getPixelColor();
            //vector<ofColor> colorData =  it->second->colorData();
            //std::copy ( colorData.begin(), colorData.end(), m_ledColors.begin() + offset);

        }
    }
}


void DisplayFadeCandy::draw()
{
    this->drawDisplayUnits();
}

void DisplayFadeCandy::drawDisplayUnits()
{
    for(DisplayUnitsMap::iterator it = m_displayUnits.begin(); it != m_displayUnits.end(); it++){
        it->second->draw();
    }
}


