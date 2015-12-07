/*
 *  HaloManager.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */


#pragma once

#include "Manager.h"
#include "HaloRing.h"
#include "DisplayFadeCandy.h"
#include "HaloVisuals.h"
#include "ofxOPC.h"

//========================== class HaloManager ==============================
//============================================================================
/** \class HaloManager HaloManager.h
 *	\brief Class managing the halo light rings
 *	\details it creates and updates the halo rings
 */


class HaloManager: public Manager
{
    static const int NUM_FADE_CANDYS;
    static const int NUM_HALO_RINGS;
    static const int NUM_HALO_LEDS;
    
    public:

        //! Constructor
        HaloManager();

        //! Destructor
        ~HaloManager();

        //! Setup the Halo Manager
        void setup();

        //! Update the Halo Manager
        void update();
    
        //! Draw the Halo Manager
        void draw();
    
        //! Set the mode of the Halo Visuals
        void setMode(int mode) {m_haloVisuals.setMode(mode);}
    
        const ofRectangle& getRingArea() const {return m_imageSpaceRectangle;}
    
        HaloVisuals& getHaloVisuals(){return m_haloVisuals;}
    
        void showRingsPreview(bool show){m_showRingsPreview=show;}

    private:
    
        void grabImageData();
    
        void setupHaloRings();
    
        void setupOPC();
    
        void createHaloRings();
    
        void createHaloRingsPositions();
    
        ofPtr<HaloRing> createSingleHaloRing(const HaloRingSettings& settings);
    
        void addHaloRing(ofPtr<HaloRing> haloRing);
    
        void updateFadeCandys();
    
        void drawDisplayFadeCandys();
    
        void drawRectangles();
    
    private:
    
        typedef             ofPtr<DisplayFadeCandy>            DisplayFadeCandyPtr;
        typedef             map<int,DisplayFadeCandyPtr>       DisplayFadeCandyMap;            ///< defines a map of Halo Rings attached to its id
        typedef             map<int,ofVec3f>                HaloRingPositionMap;    ///< defines a map of Halo Rings position attached to relative position index
    
        HaloRingPositionMap m_haloRingsPositionMap;
        HaloRingPositionMap m_haloRingsPreviewPositionMap;

        DisplayFadeCandyMap    m_DisplayFadeCandys;
        ofxOPC              m_opcClient;                    ///< instance of the Open Pixel Control client
        HaloVisuals         m_haloVisuals;
    
        // Capture Objects
        ofImage     m_screenImage;
        ofPixels    m_screenPixels;
    
        float               m_ringSize;
        float               m_ringPreviewSize;
        ofRectangle         m_imageSpaceRectangle;
        ofRectangle         m_previewRectangle;
    
        bool                m_showRingsPreview;

};



