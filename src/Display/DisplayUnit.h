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
    
    DisplayUnitSettings(): numberLeds(1), id(""), fadeCandyInd(1), channel(1), index(0){}
    
    ~DisplayUnitSettings(){}
    
    public:
    
    // Variables
    int         numberLeds;
    string      id;
    int         fadeCandyInd;
    int         channel;
    int         index;

    
};

class DisplayUnitPreview: public BasicVisual {
    
public:
    
    DisplayUnitPreview(const BasicVisual& visual, const DisplayUnitSettings& settings);
    
    virtual ~DisplayUnitPreview();
    
    virtual void draw();
    
    void setColor(const ofColor& color ) {m_ledColor = color;}
    
private:
    
    void setup();
    
    void setupTextVisual();
    
    void drawUnit();
    
    void drawID(bool hideText = false);
    

private:
    
    // Hold the Captured monochromatic colors in a RGB pixel
    ofColor m_ledColor;
    
    DisplayUnitSettings m_settings;
    
    ofPtr<TextVisual>  m_textVisual;
    
};

class DisplayUnit: public BasicVisual {
    
    public:
    
        DisplayUnit(const BasicVisual& visual, const DisplayUnitSettings& settings);
        virtual ~DisplayUnit();
    
        void setup();
        void setPixels(const ofRectangle& grabArea, const ofPixels& screenPixels);
        void draw();
    
        string getId() const {return m_settings.id;}
    
        int getChannel() const {return m_settings.channel;}
    
        int getFadeCandyNum() const {return m_settings.fadeCandyInd;}
    
        int getIndex() const {return m_settings.index;}
    
        void setDisplayUnitPreview(const BasicVisual& visual);

    
        // Return Data Method
        const ofColor& colorData(){return m_ledColor;}
    
    private:
    
        void setupDisplayUnit();
    
        void setupTextVisual();
    
        void drawGrabRegion(bool hideArea = false);
    
        void drawID(bool hideText = false);
    
    private:
            
        // Hold the Captured monochromatic colors in a RGB pixel
        ofColor m_ledColor;
    
        // Hold the Position of our Capture points
        vector <ofVec2f> m_ledPositions;
    
        DisplayUnitSettings           m_settings;
        ofPtr<DisplayUnitPreview>     m_displayUnitPreview;
    
        ofPtr<TextVisual>  m_textVisual;
    
        float m_margin;
    
};




