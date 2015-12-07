/*
 *  DisplayFadeCandy.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */------------------------------------------------------------

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

void DisplayFadeCandy::addHaloRing(ofPtr<HaloRing> haloRing)
{
    if (haloRing!= NULL) {
        m_haloRings[haloRing->getId()] = haloRing;
    }
    
}

void DisplayFadeCandy::updateHaloRings(const ofRectangle &grabArea, const ofPixels &screenPixels)
{
    for(HaloRingsMap::iterator it = m_haloRings.begin(); it != m_haloRings.end(); it++){
        
        int channel = it->second->getChannel();
        
        if (channel > 0 && channel <= FADE_CANDY_NUM_CHANNELS ) {
            int offset = (channel-1)*LEDS_PER_CHANNEL;
            
            it->second->setPixels(grabArea, screenPixels);
            vector<ofColor> colorData =  it->second->colorData();
            std::copy ( colorData.begin(), colorData.end(), m_ledColors.begin() + offset);

        }
    }
}


void DisplayFadeCandy::draw()
{
    this->drawHaloRings();
}

void DisplayFadeCandy::drawHaloRings()
{
    for(HaloRingsMap::iterator it = m_haloRings.begin(); it != m_haloRings.end(); it++){
        it->second->draw();
    }
}


