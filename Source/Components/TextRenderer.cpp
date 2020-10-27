#include "Components/TextRenderer.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"

TextRenderer::TextRenderer() {

}

TextRenderer::~TextRenderer() {

    CORE->GetSystem<GraphicsSystem>()->RemoveTextRendererComponent(Component::GetOwner()->GetID());
}

void TextRenderer::Init() {

    CORE->GetSystem<GraphicsSystem>()->AddTextRendererComponent(Component::GetOwner()->GetID(), this);
}

void TextRenderer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    (void)writer;
    /*
    writer->StartObject();

    writer->Key("component");
    writer->String("AABB");

    writer->Key("scale");
    writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

    writer->EndObject();*/
}

void TextRenderer::DeSerialize(std::stringstream& data) {

    std::cout << "Serializing AnimationRenderer" << std::endl;

    std::string model;
    std::string shdr_pgm;
    std::string font;

    data >> model
         >> shdr_pgm
         >> font
         >> text_
         >> color_.x >> color_.y >> color_.z
         >> scale_
         >> layer_
         >> ui_text_;

    model_ = CORE->GetManager<ModelManager>()->GetModel(model);
    shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm); 
    font_ = CORE->GetManager<FontManager>()->GetFont(font);
}


void TextRenderer::DeSerializeClone(std::stringstream& data)
{
    std::string model;
    std::string shdr_pgm;
    std::string font;

    data >> model
        >> shdr_pgm
        >> font
        >> text_
        >> color_.x >> color_.y >> color_.z
        >> scale_
        >> layer_
        >> ui_text_;

    model_ = CORE->GetManager<ModelManager>()->GetModel(model);
    shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
    font_ = CORE->GetManager<FontManager>()->GetFont(font);
}

std::shared_ptr<Component> TextRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning AnimationRenderer Component\n");

    std::shared_ptr<TextRenderer> cloned = std::make_shared<TextRenderer>();

    cloned->model_ = model_; // check this
    cloned->shdr_pgm_ = shdr_pgm_; // check this
    cloned->font_ = font_;
    cloned->text_ = text_;
    cloned->color_ = color_;
    cloned->scale_ = scale_;
    cloned->layer_ = layer_;
    cloned->ui_text_ = ui_text_;

    return cloned;
}