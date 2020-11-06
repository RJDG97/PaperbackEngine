#include "Components/TextRenderer.h"
#include "Manager/ComponentManager.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"

TextRenderer::TextRenderer() {

}

TextRenderer::~TextRenderer() {

    CORE->GetSystem<GraphicsSystem>()->RemoveTextRendererComponent(Component::GetOwner()->GetID());
    //CORE->GetManager<ComponentManager>()->RemoveComponent<TextRenderer>(Component::GetOwner()->GetID());
}

void TextRenderer::Init() {

    CORE->GetSystem<GraphicsSystem>()->AddTextRendererComponent(Component::GetOwner()->GetID(), this);
    //CORE->GetManager<ComponentManager>()->AddComponent<TextRenderer>(Component::GetOwner()->GetID(), this);

    font_ = CORE->GetManager<FontManager>()->GetFont(font_name_);
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

    int sentence_length;

    data >> font_name_
        >> sentence_length;

    for (int i = 0; i < sentence_length; ++i)
    {
        std::string temp;
        data >> temp;
        text_ += temp + " ";
    }

     data >> color_.x >> color_.y >> color_.z
          >> scale_
          >> layer_
          >> ui_text_;
}


void TextRenderer::DeSerializeClone(std::stringstream& data)
{
    std::string font;

    data >> font
         >> text_
         >> color_.x >> color_.y >> color_.z
         >> scale_
         >> layer_
         >> ui_text_;

    font_ = CORE->GetManager<FontManager>()->GetFont(font);
}

std::shared_ptr<Component> TextRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning AnimationRenderer Component\n");

    std::shared_ptr<TextRenderer> cloned = std::make_shared<TextRenderer>();

    cloned->font_name_ = font_name_;
    cloned->font_ = font_;
    cloned->text_ = text_;
    cloned->color_ = color_;
    cloned->scale_ = scale_;
    cloned->layer_ = layer_;
    cloned->ui_text_ = ui_text_;

    return cloned;
}