#include "Manager/LayerManager.h"
#include "Systems/Game.h"

RenderLayer::RenderLayer(std::string name, bool y_sorted, LayerType layer_type)
	: name_ {name},
	  y_sorted_ {y_sorted},
	  visible_ {1},				//will always be visible, only allowed to change this in editor
	  layer_type_ {layer_type}
{
}

std::string RenderLayer::GetName()
{
    return name_;
}

LayerType RenderLayer::GetLayerType()
{
	return layer_type_;
}

bool RenderLayer::GetVisible()
{
	return visible_;
}

void RenderLayer::SetVisible(bool visible)
{
	visible_ = visible;
}

bool RenderLayer::GetYSorted()
{
    return y_sorted_;
}

void RenderLayer::SetYSorted(bool y_sorted)
{
    y_sorted_ = y_sorted;
}

void RenderLayer::AddRenderer(IRenderer* irenderer, int order_in_layer)
{
    renderers_.insert({ static_cast<float>(order_in_layer), irenderer });
}

void RenderLayer::RemoveRenderer(IRenderer* irenderer)
{
    for (auto it = renderers_.begin();
        it != renderers_.end(); ++it)
    {
        if (it->second == irenderer)
        {
            renderers_.erase(it);
            return;
        }
    }
}

std::multimap<float, IRenderer*>* RenderLayer::GetRenderers()
{
	return &renderers_;
}

void LayerManager::Init()
{
}

void LayerManager::Update(float frametime)
{
    (void) frametime;
}

void LayerManager::LoadLevelLayers(std::string level_name)
{
    render_layers_.clear(); //clears all previous layers

    rapidjson::Document level_layers;
    std::string path = "Resources/Layers/" + level_name + "_layers.json";

    DeserializeJSON(path, level_layers);

    const rapidjson::Value& files_arr = level_layers;
    DEBUG_ASSERT(files_arr.IsObject(), "Dialogue JSON does not exist in proper format");

    //handle sorting of information into the map
    for (rapidjson::Value::ConstMemberIterator file_it = files_arr.MemberBegin(); file_it != files_arr.MemberEnd(); ++file_it) {

        std::string layer_name { file_it->name.GetString() };
        std::stringstream layer_info { file_it->value.GetString() };

        bool y_sorted;
        std::string layer_type;
        layer_info >> y_sorted >> layer_type;

        bool invalid = false;

        if (layer_type == "UI_Sprite") {

            render_layers_[static_cast<int>(render_layers_.size())] = { layer_name, y_sorted, UI_SPRITE };
        }

        else if (layer_type == "UI_Text") {

            render_layers_[static_cast<int>(render_layers_.size())] = { layer_name, y_sorted, UI_TEXT };
        }

        else if (layer_type == "World_Sprite") {

            render_layers_[static_cast<int>(render_layers_.size())] = { layer_name, y_sorted, WORLD_SPRITE };
        }

        else if (layer_type == "World_Text") {

            render_layers_[static_cast<int>(render_layers_.size())] = { layer_name, y_sorted, WORLD_TEXT };
        }

        else {

            invalid = true;
            DEBUG_ASSERT(invalid, "Invalid Layer Type!");
        }
    }
}

void LayerManager::DeserializeJSON(const std::string& filename, rapidjson::Document& doc)
{
    std::ifstream input_file(filename.c_str());
    DEBUG_ASSERT(input_file.is_open(), "File does not exist");

    // Read each line separated by a '\n' into a stringstream
    std::stringstream json_doc_buffer;
    std::string input;

    while (std::getline(input_file, input)) {

        json_doc_buffer << input << "\n";
    }

    // Close the file (.json) after
    input_file.close();

    // Parse the stringstream into document (DOM) format
    doc.Parse(json_doc_buffer.str().c_str());
}

void LayerManager::AddLayer(std::string layer_name, LayerType layer_type)
{
    render_layers_[render_layers_.size()] = { layer_name, false, layer_type };
}

void LayerManager::DeleteLayer(int layer_position)
{
    std::map<int, RenderLayer> temp;
    
    for (auto it = render_layers_.begin(); it != render_layers_.end(); ++it)
    {
        if (it->first != layer_position)
        {
            temp[temp.size()] = it->second;
        }
    }

    render_layers_ = temp;
}

void LayerManager::SwapLayer(int layer_position_1, int layer_position_2)
{
    RenderLayer temp = render_layers_[layer_position_1];
    render_layers_[layer_position_1] = render_layers_[layer_position_2];
    render_layers_[layer_position_2] = temp;
}

std::map<int, RenderLayer>* LayerManager::GetRenderLayers()
{
	return &render_layers_;
}
