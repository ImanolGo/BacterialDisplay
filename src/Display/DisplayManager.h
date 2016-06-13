/*
 *  DisplayManager.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */


#pragma once

#include "Manager.h"
#include "DisplayUnit.h"
#include "DisplayFadeCandy.h"
#include "DisplayVisuals.h"
#include "ofxOPC.h"

//========================== class DisplayManager ==============================
//============================================================================
/** \class DisplayManager DisplayManager.h
 *	\brief Class managing the halo light rings
 *	\details it creates and updates the halo rings
 */


class DisplayManager: public Manager
{
    
    public:

        //! Constructor
        DisplayManager();

        //! Destructor
        ~DisplayManager();

        //! Setup the Halo Manager
        void setup();

        //! Update the Halo Manager
        void update();
    
        //! Draw the Halo Manager
        void draw();
    
        //! Set the mode of the Halo Visuals
        void setMode(int mode) {m_displayVisuals.setMode(mode);}
    
        const ofRectangle& getDisplayArea() const {return m_imageSpaceRectangle;}
    
        DisplayVisuals& getDisplayVisuals(){return m_displayVisuals;}
    
        void showDisplayPreview(bool show){m_showDisplayPreview=show;}
    
        void onFlickeringCHange(float & value);

    private:
    
        void closeOPC();
    
        void grabImageData();
    
        void setupDisplayUnits();
    
        void setupOPC();
    
        void createDisplayUnitSettings();
    
        void createDisplayUnits();
    
        ofPtr<DisplayUnit> createSingleDisplayUnit(int id);
    
        void addDisplayUnit(ofPtr<DisplayUnit> DisplayUnit);
    
        void updateFadeCandys();
    
        void drawDisplayFadeCandys();
    
        void drawRectangles();
    
    private:
    
        typedef             ofPtr<DisplayFadeCandy>            DisplayFadeCandyPtr;
        typedef             map<int,DisplayFadeCandyPtr>       DisplayFadeCandyMap;            ///< defines a map of Display Unit attached to its id
        typedef             map<int,ofVec3f>                   DisplayUnitPositionMap;    ///< defines a map of Display Unit position attached to relative id
        typedef             map<int,DisplayUnitSettings>       DisplayUnitSettingsMap;    ///< defines a map of Display Unit settings attached to relative id 
    
        DisplayUnitPositionMap m_displayUnitsPositionMap;
        DisplayUnitPositionMap m_displayUnitsPreviewPositionMap;
        DisplayUnitSettingsMap m_displayUnitsSettings;

        DisplayFadeCandyMap    m_displayFadeCandys;
        ofxOPC                 m_opcClient;                    ///< instance of the Open Pixel Control client
        DisplayVisuals         m_displayVisuals;
    
        // Capture Object
        ofPixels    m_screenPixels;
    
        float               m_displayUnitSize;
        float               m_displayUnitPreviewSize;
        ofRectangle         m_imageSpaceRectangle;
        ofRectangle         m_previewRectangle;
    
        float               m_flickering;
    
        bool                m_showDisplayPreview;

};



