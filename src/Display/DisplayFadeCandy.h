/*
 *  DisplayFadeCandy.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */


#pragma once

#include "ofMain.h"

#include "DisplayUnit.h"



class DisplayFadeCandy {
    
    public:
    
        DisplayFadeCandy(int id);
    
        virtual ~DisplayFadeCandy();
    
        void addDisplayUnit(ofPtr<DisplayUnit> displayUnit);
    
        void updateDisplayUnits(const ofRectangle& grabArea, const ofPixels& screenPixels);
    
        void setFlickering(float flickering);
    
        void draw();
    
        int getId(){return m_id;}
    
        const vector <ofColor>& getColorData(){return m_ledColors;}
    
    private:
    
        void setup();
    
        void createLedColorVector();
    
        void drawDisplayUnits();
    
    private:
    
        typedef std::map < string, ofPtr<DisplayUnit> > DisplayUnitsMap;
    
        DisplayUnitsMap m_displayUnits;
    
        vector <ofColor> m_ledColors;

        int m_id;
    
};




