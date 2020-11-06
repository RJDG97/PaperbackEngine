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
    //CORE->GetManager<ComponentManager>()->RemoveComponent<TextureRenderer>(Component::GetOwner()->GetID());
}

void TextureRenderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddTextureRendererComponent(Component::GetOwner()->GetID(), this);
    //CORE->GetManager<ComponentManager>()->AddComponent<TextureRenderer>(Component::GetOwner()->GetID(), this);
    texture_ = *CORE->GetManager<TextureManager>()->GetTexture(texture_name_);
    texture_handle_ = texture_.GetTilesetHandle();
    tex_vtx_ = *texture_.GetTexVtx();
}

void TextureRenderer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    (void)writer;
    /*
    writer->StartObject();

    writer->Key("component");
    writer->String("AABB");

    writer->Key("scale");
    writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

    writer->EndObject();*/
}

void TextureRenderer::DeSerialize(std::stringstream& data) {
    
    data >> texture_name_ >> layer_;
}

std::shared_ptr<Component> TextureRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning TextureRenderer Component\n");

    std::shared_ptr<TextureRenderer> cloned = std::make_shared<TextureRenderer>();

    // IRenderer
    cloned->layer_ = layer_;

    // TextureRenderer
    cloned->texture_name_ = texture_name_;
    cloned->texture_ = texture_;

    return cloned;
}