#include "Components/TextureRenderer.h"
#include "Manager/ComponentManager.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

TextureRenderer::TextureRenderer() {
    
}

TextureRenderer::~TextureRenderer() {
    
    CORE->GetSystem<GraphicsSystem>()->RemoveTextureRendererComponent(Component::GetOwner()->GetID());
}

void TextureRenderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddTextureRendererComponent(Component::GetOwner()->GetID(), this);

    InitTextures();
}

void TextureRenderer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    writer->StartObject();

    writer->Key("component");
    writer->String("TextureRenderer");

    writer->Key("texture");
    writer->String(texture_name_.c_str());

    writer->Key("layer");
    writer->String(std::to_string(layer_).c_str());

    writer->Key("ui");
    writer->String(std::to_string(ui_).c_str());

    writer->EndObject();
}

void TextureRenderer::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    Serialize(writer);
}

void TextureRenderer::DeSerialize(std::stringstream& data) {
    
    data >> texture_name_ >> layer_ >> ui_;
}

void TextureRenderer::DeSerializeClone(std::stringstream& data) {

    //remove existing entry before proceeding
    CORE->GetSystem<GraphicsSystem>()->RemoveTextureRendererComponent(Component::GetOwner()->GetID());

    int layer;

    data >> texture_name_ >> layer >> ui_;

    if (layer != layer_) {

        M_DEBUG->WriteDebugMessage("Layer is keyed in wrongly!");
    }

    //readd with new info
    CORE->GetSystem<GraphicsSystem>()->AddTextureRendererComponent(Component::GetOwner()->GetID(), this);
    InitTextures();
}

std::shared_ptr<Component> TextureRenderer::Clone() {

    M_DEBUG->WriteDebugMessage("Cloning TextureRenderer Component\n");

    std::shared_ptr<TextureRenderer> cloned = std::make_shared<TextureRenderer>();

    // IRenderer
    cloned->layer_ = layer_;
    cloned->ui_ = ui_;

    // TextureRenderer
    cloned->texture_name_ = texture_name_;
    cloned->texture_ = texture_;

    return cloned;
}

void TextureRenderer::InitTextures() {

    texture_ = *CORE->GetManager<TextureManager>()->GetTexture(texture_name_);
    texture_handle_ = texture_.GetTilesetHandle();
    tex_vtx_ = *texture_.GetTexVtx();
}