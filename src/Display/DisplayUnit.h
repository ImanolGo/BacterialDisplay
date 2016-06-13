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
#include "ImageVisual.h"

enum class DisplayUnitOrientation {
    UP,
    DOWN
};

class DisplayUnitSettings {
    
    public:
    
    DisplayUnitSettings(): numberLeds(1), id(""), fadeCandyInd(1), channel(1), index(0), orientation(DisplayUnitOrientation::UP){}
    
    ~DisplayUnitSettings(){}
    
    public:
    
    // Variables
    int         numberLeds;
    string      id;
    int         fadeCandyInd;
    int         channel;
    int         index;
    ofPoint     pos;
    DisplayUnitOrientation  orientation;
    
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
    
    void setupImageVisuals();
    
    void drawUnit();
    
    void drawID(bool hideText = false);
    

private:
    
    // Hold the Captured monochromatic colors in a RGB pixel
    ofColor m_ledColor;
    
    DisplayUnitSettings m_settings;
    
    ofPtr<TextVisual>  m_textVisual;
    ImageVisual        m_ringVisual;
    ImageVisual        m_ledVisual;

    
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
    
        void setFlickering(float value){m_flickering = value;}

    
        // Return Data Method
        const ofColor& getPixelColor(){return m_ledColor;}
    
    private:
    
        void setupDisplayUnit();
    
        void setupTextVisual();
    
        void drawGrabRegion(bool hideArea = false);
    
        void drawID(bool hideText = false);
    
        float getNoise(int index);
    
    private:
            
        // Hold the Captured monochromatic colors in a RGB pixel
        ofColor m_ledColor;
    
        // Hold the Position of our Capture points
        vector <ofVec2f> m_ledPositions;
    
        DisplayUnitSettings           m_settings;
        ofPtr<DisplayUnitPreview>     m_displayUnitPreview;
    
        ofPtr<TextVisual>  m_textVisual;
    
        float m_margin;
        float m_flickering;
    
};




