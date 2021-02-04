#pragma once

#include "Manager/IManager.h"
#include "Components/IRenderer.h"

enum LayerType
{
    UI_SPRITE,
    UI_TEXT,
    WORLD_SPRITE,
    WORLD_TEXT
};

class RenderLayer {

    std::string name_;
    bool y_sorted_;
    bool visible_;                                 // used to show/hide a layer in the editor.
    std::multimap<float, IRenderer*> renderers_;

    LayerType layer_type_;

public:
    
    RenderLayer() = default;
    RenderLayer(std::string name, bool y_sorted, LayerType layer_type_);

    std::string GetName();
    LayerType GetLayerType();
    bool GetVisible();
    void SetVisible(bool visible);
    bool GetYSorted();
    void SetYSorted(bool y_sorted);
    void AddRenderer(IRenderer* irenderer, int order_in_layer);
    void RemoveRenderer(IRenderer* irenderer);
    std::multimap<float, IRenderer*>* GetRenderers();
};

class LayerManager : public IManager {
    
    std::map<int, RenderLayer> render_layers_;

public:

    void Init();
    void Update(float frametime);
    void LoadLevelLayers(std::string level_name);
    void DeserializeJSON(const std::string& filename, rapidjson::Document& doc);

    std::map<int, RenderLayer>* GetRenderLayers();
};