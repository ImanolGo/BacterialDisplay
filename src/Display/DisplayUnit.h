/*
 *  DisplayUnit.cpp
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */


#pragma once

#include "ofMain.h"
#include "BasicVisual.h"
#include "TextVisual.h"

class DisplayUnitSettings {
    
    public:
    
    DisplayUnitSettings(): numberLeds(0), id(""), fadeCandyInd(1), channel(1), positionIndex(0){}
    
    ~DisplayUnitSettings(){}
    
    public:
    
    // Variables
    int         numberLeds;
    string      id;
    int         fadeCandyInd;
    int         channel;
    int         positionIndex;

    
};

class DisplayUnitPreview: public BasicVisual {
    
public:
    
    DisplayUnitPreview(const BasicVisual& visual, string id);
    
    virtual ~DisplayUnitPreview();
    
    virtual void draw();
    
    void setColors(const vector <ofColor>& colors ) {m_ledColors = colors;}
    
private:
    
    void setup();
    
    void setupTextVisual();
    
    void drawLedRing();
    
    void drawID(bool hideText = false);
    
private:
    
    vector <ofColor> m_ledColors;
    
    string     m_id;
    
    ofPtr<TextVisual>  m_textVisual;
    
};

class DisplayUnit: public BasicVisual {
    
    public:
    
        DisplayUnit(const BasicVisual& visual, const DisplayUnitSettings& settings);
        virtual ~DisplayUnit();
    
        void setup();
        void setPixels(const ofRectangle& grabArea, const ofPixels& screenPixels);
        void draw();
    
        string getId(){return m_settings.id;}
    
        int getChannel(){return m_settings.channel;}
    
        int getFadeCandyNum(){return m_settings.fadeCandyInd;}
    
        void setDisplayUnitPreview(const BasicVisual& visual);

    
        // Return Data Method
        const vector <ofColor>& colorData(){return m_ledColors;}
    
    private:
    
        void setupLedRing();
    
        void setupTextVisual();
    
        void drawGrabRegion(bool hideArea = false);
    
        void drawID(bool hideText = false);
    
    private:
            
        // Hold the Captured Colors
        vector <ofColor> m_ledColors;
    
        // Hold the Position of our Capture points
        vector <ofVec2f> m_ledPositions;
    
        DisplayUnitSettings            m_settings;
        ofPtr<DisplayUnitPreview>     m_DisplayUnitPreview;
    
        ofPtr<TextVisual>  m_textVisual;
    
        float m_margin;
    
};




