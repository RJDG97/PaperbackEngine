#pragma once

#include "Components/IRenderer.h"

class RenderLayer {

    std::string name_;
    bool hidden;        // used in editor to show/hide layer

};

class WorldLayer : public RenderLayer {

    bool y_sorted;
    std::multimap<int, IRenderer*> renderers_;
};

class UILayer : public RenderLayer {

    std::multimap<int, IRenderer*> renderers_;
};


class LayerManager : public IManager {

};