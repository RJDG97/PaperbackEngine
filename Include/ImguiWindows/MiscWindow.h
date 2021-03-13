/**********************************************************************************
*\file         MiscWindow.h
*\brief        Contains declaration of functions and variables used for
*			   the Miscellaneous Windows (eg. Layer Window and Editor Settings)

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef _MISC_WINDOW_H_
#define _MISC_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/LayerManager.h"
#include "Engine/Core.h"
#include "ImguiWindows/AssetConsoleWindow.h"
#include <filesystem>

namespace fs = std::filesystem;

class MiscWindow : public IWindow {

public:

    /******************************************************************************/
    /*!
        \fn Init()

        \brief Initializes the ImGui System
    */
    /******************************************************************************/
    void Init() override;

    /******************************************************************************/
    /*!
        \fn Update()

        \brief Updates all ImGui Windows in the system
    */
    /******************************************************************************/
    void Update() override;

    /******************************************************************************/
    /*!
        \fn DragEntityCheckBox()

        \brief To enable or disable users to be able to drag entities around
    */
    /******************************************************************************/
    void DragEntityCheckBox();

    /******************************************************************************/
    /*!
        \fn EditorSettingsWindow()

        \brief Contains the different options for the editor
    */
    /******************************************************************************/
    void EditorSettingsWindow();

    /******************************************************************************/
    /*!
        \fn GraphicsLayerWindow()

        \brief Shows the user the available layers, and allows users to edit layers
    */
    /******************************************************************************/
    void GraphicsLayerWindow();

    /******************************************************************************/
    /*!
        \fn LayerWindowMenuBar()

        \brief Menu Bar for the Layer Window in the Editor
    */
    /******************************************************************************/
    void LayerWindowMenuBar();

    /******************************************************************************/
    /*!
        \fn GraphicsLayerWindowMenuBar()

        \brief Get the layer types available in the engine
    */
    /******************************************************************************/
    std::string GetLayerType(int layer);

    /******************************************************************************/
    /*!
        \fn SerializeLayers()

        \brief Serialize the newly added layers into the json
    */
    /******************************************************************************/
    void SerializeLayers();

    /******************************************************************************/
    /*!
        \fn LayerDropDown()

        \brief Handles the drop down for choosing layers
    */
    /******************************************************************************/
    void LayerDropDown();

    /******************************************************************************/
    /*!
        \fn LayerList()

        \brief Display the list of layers and their buttons
    */
    /******************************************************************************/
    void LayerList();

    /******************************************************************************/
    /*!
        \fn NewLayerPopUp()

        \brief Handles the pop up for adding new layers
    */
    /******************************************************************************/
    void NewLayerPopUp();

    /******************************************************************************/
    /*!
        \fn LayerPopUps()

        \brief Handles the pop up for the layer window
    */
    /******************************************************************************/
    void LayerPopUps();

    /******************************************************************************/
    /*!
        \fn RemoveLayerPopUp()

        \brief Handles the pop up for adding new layers
    */
    /******************************************************************************/
    bool CheckLayerEmpty(int indexcheck);

private:

    ImguiSystem* imgui_;
    LayerManager* layer_;
    GraphicsSystem* graphics_;
    InputSystem* input_;
    AssetConsoleWindow* assetconsole_;

    bool b_draw, b_grid, b_light, b_layers, b_Nlayer, b_swap, b_remove, b_notempty;

    Vector2D mousePos_;

    std::string newlayertype_, layerjson_load_, label_, layername_;
   
    size_t newlayerint, renderercount;

    int layerDelete_;

    const char* layerType_[5] = {"UI_Sprite", "UI_Text", "World_Sprite", "World_Text", "Vignette"};
};

#endif //MISC_WINDOW_H