/*
 *  AppManager.h
 *  Bacterial Display
 *
 *  Created by Imanol Gomez on 07/12/15.
 *
 */

#pragma once

#include "ofMain.h"

#include "Manager.h"

#include "SettingsManager.h"
#include "ResourceManager.h"
#include "ViewManager.h"
#include "VisualEffectsManager.h"
#include "LayoutManager.h"
#include "KeyboardManager.h"
#include "DisplayManager.h"
#include "CameraTrackingManager.h"
#include "GuiManager.h"

//========================== class AppManager ==============================
//============================================================================
/** \class AppManager AppManager.h
 *	\brief Class managing the whole application
 *	\details it set-ups, updates and renders all the different managers used for the application
 */

class AppManager: public Manager
{
public:

    //! Destructor
    ~AppManager();

    //! Returns the singleton instance.
    static AppManager& getInstance();

    //! Compares two transition objects
    void setup();

    //! updates the logic
    void update();

    //! calls the view manager to draw
    void draw();

    //==========================================================================

    //! Returns the settings manager
    ofPtr<SettingsManager> getSettingsManager() { return m_settingsManager; }
    
    //! Returns the resource manager
    ofPtr<ResourceManager> getResourceManager() { return m_resourceManager; }
    
    //! Returns the view manager
    ofPtr<ViewManager>    getViewManager() { return m_viewManager; }
    
    //! Returns the visual effects manager
    ofPtr<VisualEffectsManager>   getVisualEffectsManager() { return m_visualEffectsManager; }
    
    //! Returns the visual halo manager
    ofPtr<DisplayManager>   getDisplayManager() { return m_displayManager; }
    
    //! Returns the visual camera tracking manager
    ofPtr<CameraTrackingManager>   getCameraTrackingManager() { return m_cameraTrackingManager; }
    
    //! Returns the GUI manager
    ofPtr<GuiManager>   getGuiManager() { return m_guiManager; }

    
    //==========================================================================
    
    void toggleDebugMode();
    
    void setDebugMode(bool showDebug);
    

private:

     //! Constructor
     AppManager();

	//! Stop the compiler generating methods of copy the object
	 AppManager(AppManager const& copy);              // Don't Implement

    //! Prevent operator= being generated.
     AppManager& operator=(AppManager const& copy);     // Don't implement

    //==========================================================================

    //! Create all the managers with shared pointers
    void createManagers();

    //! Set-up all the managers
    void setupManagers();

    //! Set-up openFrameworks
    void setupOF();

    //! update all the managers
    void updateManagers();


private:

    ofPtr<SettingsManager>          m_settingsManager;          ///< Manages the application's settings
    ofPtr<ResourceManager>          m_resourceManager;          ///< Manages the application's resources
    ofPtr<ViewManager>              m_viewManager;              ///< Manages visuals
    ofPtr<VisualEffectsManager>     m_visualEffectsManager;     ///< Manages visual effects
    ofPtr<LayoutManager>            m_layoutManager;            ///< Manages the layout
    ofPtr<KeyboardManager>          m_keyboardManager;          ///< Manages the keyboard
    ofPtr<DisplayManager>           m_displayManager;           ///< Manages the display
    ofPtr<CameraTrackingManager>    m_cameraTrackingManager;    ///< Manages the camera tracking
    ofPtr<GuiManager>               m_guiManager;               ///< Manages the graphical user interface

    bool                            m_debugMode;
};

//==========================================================================


