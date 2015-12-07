/*
 *  DisplayFadeCandy.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */


#pragma once

#include "ofMain.h"

#include "HaloRing.h"



class DisplayFadeCandy {
    
    public:
    
        DisplayFadeCandy(int id);
    
        virtual ~DisplayFadeCandy();
    
        void addHaloRing(ofPtr<HaloRing> haloRing);
    
        void updateHaloRings(const ofRectangle& grabArea, const ofPixels& screenPixels);
    
        void draw();
    
        int getId(){return m_id;}
    
        const vector <ofColor>& getColorData(){return m_ledColors;}
    
    private:
    
        void setup();
    
        void createLedColorVector();
    
        void drawHaloRings();
    
    private:
    
        typedef std::map < string, ofPtr<HaloRing> > HaloRingsMap;
    
        HaloRingsMap m_haloRings;
    
        vector <ofColor> m_ledColors;

        int m_id;
    
};




