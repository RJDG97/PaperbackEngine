#pragma once
#ifndef _MISC_WINDOW_H_
#define _MISC_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/LayerManager.h"
#include "Engine/Core.h"

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

private:

    ImguiSystem* imgui_;
    LayerManager* layer_;
    GraphicsSystem* graphics_;
    InputSystem* input_;

    bool b_draw, b_grid, b_light, b_layers;
    Vector2D mousePos_;
};

#endif //MISC_WINDOW_H