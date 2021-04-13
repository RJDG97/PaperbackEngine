#pragma once

#include "Manager/IManager.h"
#include "Components/IRenderer.h"

enum LayerType
{
    UI_SPRITE,
    UI_TEXT,
    WORLD_SPRITE,
    WORLD_TEXT,
    VIGNETTE              //used for vignettes, blurs, etc.
};

class RenderLayer {

    std::string name_;
    bool y_sorted_;
    bool visible_;                                 // used to show/hide a layer in the editor.
    std::multimap<float, IRenderer*> renderers_;

    LayerType layer_type_;

public:
   
    RenderLayer() = default;

    /******************************************************************************/
    /*!
      \fn RenderLayer(std::string name, bool y_sorted, LayerType layer_type_)

      \brief Constructor for render layer
    */
    /******************************************************************************/
    RenderLayer(std::string name, bool y_sorted, LayerType layer_type_);

    /******************************************************************************/
    /*!
      \fn GetName()

      \brief Gets the name of the layer
    */
    /******************************************************************************/
    std::string GetName();

    /******************************************************************************/
    /*!
      \fn GetLayerType()

      \brief Gets the type of the layer
    */
    /******************************************************************************/
    LayerType GetLayerType();

    /******************************************************************************/
    /*!
      \fn GetVisible()

      \brief Gets the visibility of the layer
    */
    /******************************************************************************/
    bool GetVisible();

    /******************************************************************************/
    /*!
      \fn SetVisible()

      \brief Sets the visibility of the layer
    */
    /******************************************************************************/
    void SetVisible(bool visible);

    /******************************************************************************/
    /*!
      \fn GetYSorted()

      \brief Checks if the layer needs to be y-sorted
    */
    /******************************************************************************/
    bool GetYSorted();

    /******************************************************************************/
    /*!
      \fn SetYSorted()

      \brief Sets if the layer needs to be y-sorted
    */
    /******************************************************************************/
    void SetYSorted(bool y_sorted);

    /******************************************************************************/
    /*!
      \fn AddRenderer(IRenderer* irenderer, int order_in_layer)

      \brief Adds a renderer into the layer
    */
    /******************************************************************************/
    void AddRenderer(IRenderer* irenderer, int order_in_layer);

    /******************************************************************************/
    /*!
      \fn RemoveRenderer(IRenderer* irenderer)

      \brief Removes the renderer from the layer
    */
    /******************************************************************************/
    void RemoveRenderer(IRenderer* irenderer);

    /******************************************************************************/
    /*!
      \fn GetRenderers()

      \brief Gets all renderers from this layer
    */
    /******************************************************************************/
    std::multimap<float, IRenderer*>* GetRenderers();
};

class LayerManager : public IManager {
    
    std::map<int, RenderLayer> render_layers_;

public:

    /******************************************************************************/
    /*!
      \fn Init()

      \brief Initializes the layer manager
    */
    /******************************************************************************/
    void Init();

    /******************************************************************************/
    /*!
      \fn Update(float frametime)

      \brief Updates the layer manager
    */
    /******************************************************************************/
    void Update(float frametime);

    /******************************************************************************/
    /*!
      \fn LoadLevelLayers(std::string level_name)

      \brief Loads all layers used within the level
    */
    /******************************************************************************/
    void LoadLevelLayers(std::string level_name);

    /******************************************************************************/
    /*!
      \fn DeserializeJSON(const std::string& filename, rapidjson::Document& doc)

      \brief Deserializes the level layer json
    */
    /******************************************************************************/
    void DeserializeJSON(const std::string& filename, rapidjson::Document& doc);

    /******************************************************************************/
    /*!
      \fn AddLayer(std::string layer_name, LayerType layer_type)

      \brief Adds a layer to the level
    */
    /******************************************************************************/
    void AddLayer(std::string layer_name, LayerType layer_type);

    /******************************************************************************/
    /*!
      \fn DeleteLayer(int layer_position)

      \brief Deletes a layer from the level
    */
    /******************************************************************************/
    void DeleteLayer(int layer_position);

    /******************************************************************************/
    /*!
      \fn SwapLayer(int layer_position_1, int layer_position_2)

      \brief Swaps two layers
    */
    /******************************************************************************/
    void SwapLayer(int layer_position_1, int layer_position_2);

    /******************************************************************************/
    /*!
      \fn GetRenderLayers()

      \brief Gets all the layers from the level
    */
    /******************************************************************************/
    std::map<int, RenderLayer>* GetRenderLayers();
};