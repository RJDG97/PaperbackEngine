#define _USE_MATH_DEFINES

#include "Systems/GraphicsSystem.h"
#include "Engine/Core.h"
#include <FreeImage.h>
#include <cmath>
#include <string>
#include <set>
#include "Manager/ModelManager.h"
#include "Entity/ComponentTypes.h"
#include "Entity/ComponentCreator.h"
#include "Systems/Debug.h"
#include "Systems/Message.h"
#include "Systems/LightingSystem.h"
#include "Components/Scale.h"
#include "Components/Transform.h"
#include <glm/gtc/type_ptr.hpp>

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Init

@param none

@return void

Initializes the OpenGL rendering pipeline, and then prints the OpenGL
Context information.
*/
void GraphicsSystem::Init() {

    ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

    text_renderer_arr_ = comp_mgr->GetComponentArray<TextRenderer>();
    texture_renderer_arr_ = comp_mgr->GetComponentArray<TextureRenderer>();
    anim_renderer_arr_ = comp_mgr->GetComponentArray<AnimationRenderer>();

    // Clear colorbuffer with cyan color ...
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    windows_system_ = CORE->GetSystem<WindowsSystem>();
    camera_system_ = CORE->GetSystem<CameraSystem>();

    // Set up viewports
    win_size_.x = static_cast<float>(windows_system_->GetWinWidth());
    win_size_.y = static_cast<float>(windows_system_->GetWinHeight());
    glViewport(0, 0, static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y));

    // Set up frame buffer for rendering all objects to texture
    glGenFramebuffers(1, &frame_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

    glGenTextures(1, &final_texture_);
    glBindTexture(GL_TEXTURE_2D, final_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, final_texture_, 0);

    glGenRenderbuffers(1, &render_buffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                          static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_);

    DEBUG_ASSERT(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE),
                 "Final framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Set up all models and shaders
    model_manager_ = CORE->GetManager<ModelManager>();
    shader_manager_ = CORE->GetManager<ShaderManager>();
    font_manager_ = CORE->GetManager<FontManager>();
    texture_manager_ = CORE->GetManager<TextureManager>();
    animation_manager_ = CORE->GetManager<AnimationManager>();
    component_manager_ = CORE->GetManager<ComponentManager>();

    batch_size_ = 500;
    graphic_models_["BoxModel"] = model_manager_->AddTristripsModel(1, 1, "BoxModel");
    graphic_models_["TextModel"] = model_manager_->AddTristripsModel(1, 1, "TextModel");
    graphic_models_["BatchModel"] = model_manager_->AddTristripsBatchModel(batch_size_, "BatchModel");

    graphic_shaders_["ObjectShader"] =
        shader_manager_->AddShdrpgm("Shaders/world_object.vert","Shaders/world_object.frag", "ObjectShader");

    graphic_shaders_["TextShader"] =
        shader_manager_->AddShdrpgm("Shaders/text.vert", "Shaders/text.frag", "TextShader");

    graphic_shaders_["FinalShader"] =
        shader_manager_->AddShdrpgm("Shaders/final.vert", "Shaders/final.frag", "FinalShader");

    lighting_texture_ = CORE->GetSystem<LightingSystem>()->GetLightingTexture();

    //For UI
    projection = glm::ortho(0.0f, win_size_.x, 0.0f, win_size_.y);

    M_DEBUG->WriteDebugMessage("Graphics System Init\n");
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::update

@param double delta_time

@return void

This also updates the size of the squares to be rendered in one of the tasks.
*/
void GraphicsSystem::Update(float frametime) {
    
    if (debug_) { M_DEBUG->WriteDebugMessage("\nGraphics System Update Debug Log:\n"); }

    for (AnimRendererIt it = anim_renderer_arr_->begin(); it != anim_renderer_arr_->end(); ++it) {

        if (debug_) {
            // Log id of entity and it's updated components that are being updated
            M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) +
                " (Scale, Rotation, Translation matrix & Texture animation updated)\n");
        }
        
        //UpdateObjectMatrix(it->second, world_to_ndc_xform_);
        UpdateAnimationFrame(it->second, frametime);
    }

}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GraphicsSystem::Draw() {

    if (debug_) { M_DEBUG->WriteDebugMessage("\nGraphics System Draw Debug Log:\n"); }

    glm::vec2 cam_pos_ = camera_system_->cam_pos_;
    glm::mat3 world_to_ndc_xform = camera_system_->world_to_ndc_xform_;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    graphic_shaders_["ObjectShader"]->Use();
    glBindVertexArray(graphic_models_["BatchModel"]->vaoid_);
    GLuint vbo_hdl = graphic_models_["BatchModel"]->vboid_;

    //draws all the world textures/animations
    for (WorldRenderOrderIt it = worldobj_renderers_in_order_.begin();
         it != worldobj_renderers_in_order_.end() ; ) {

        if (debug_) {
			// Log id of entity and its updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing entity: " +
                std::to_string(it->first) + "\n");
		}

        BatchWorldObject(it->second);
        
        int current_layer = it->second->layer_;
        auto next_object = ++it;

        if (tex_vtx_sent.size() == static_cast<size_t>(batch_size_) * 4 ||
            next_object == worldobj_renderers_in_order_.end() ||
            next_object->second->layer_ != current_layer)
        {
            DrawBatch(vbo_hdl, world_to_ndc_xform);
        }
    
    }

    graphic_shaders_["TextShader"]->Use();
    glBindVertexArray(graphic_models_["TextModel"]->vaoid_);

    //draws all the world text
    for (TextRenderOrderIt it = worldtext_renderers_in_order_.begin();
         it != worldtext_renderers_in_order_.end(); ++it) {

        if (debug_) {
            // Log id of entity and its updated components that are being updated
            M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
        }

        DrawTextObject(graphic_shaders_["TextShader"], graphic_models_["TextModel"], it->second, cam_pos_);
    }

    graphic_shaders_["FinalShader"]->Use();
    glBindVertexArray(graphic_models_["BoxModel"]->vaoid_);
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    DrawFinalTexture(graphic_models_["BoxModel"], graphic_shaders_["FinalShader"], lighting_texture_);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    DrawFinalTexture(graphic_models_["BoxModel"], graphic_shaders_["FinalShader"], &final_texture_);

    //draws all the UI text
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    graphic_shaders_["TextShader"]->Use();
    glBindVertexArray(graphic_models_["TextModel"]->vaoid_);

    for (TextRenderOrderIt it = uitext_renderers_in_order_.begin();
         it != uitext_renderers_in_order_.end(); ++it) {

        if (debug_) {
            // Log id of entity and its updated components that are being updated
            M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
        }

        DrawTextObject(graphic_shaders_["TextShader"], graphic_models_["TextModel"], it->second, cam_pos_);
    }

    if (debug_) { debug_ = !debug_; }
}

void GraphicsSystem::DrawFinalTexture(Model* model, Shader* shader, GLuint* texture) {

    glBindTexture(GL_TEXTURE_2D, *texture);
    glBindTextureUnit(0, *texture);
    glUseProgram(shader->GetHandle());
  
    shader->SetUniform("uTex2d", 0);

    glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Cleanup

@param none

@return void

Returns allocated resources
*/
void GraphicsSystem::CleanUp() {

    glDeleteFramebuffers(1, &frame_buffer_);
    glDeleteRenderbuffers(1, &render_buffer_);
    glDeleteTextures(1, &final_texture_);
}

std::string GraphicsSystem::GetName() {

    return "GraphicsSystem";
}

void GraphicsSystem::SendMessageD(Message* m) {

    AnimRendererIt player_renderer;

    for (player_renderer = anim_renderer_arr_->begin();
         player_renderer != anim_renderer_arr_->end();
         ++player_renderer)
    {
        if (ENTITYNAME(player_renderer->second->GetOwner()) == "Player")
        {
            break;
        }
    }

    if (player_renderer == anim_renderer_arr_->end())
    {
        return;
    }

    switch(m->message_id_) {

        case MessageIDTypes::DEBUG_ALL: {
            debug_ = true;
            break;
        }
        
        case MessageIDTypes::CAM_UPDATE_POS: {
            //placeholder name for message, will be changed after engineproof
            MessagePhysics_Motion* msg = dynamic_cast<MessagePhysics_Motion*>(m);
            camera_system_->TempCameraMove(msg->new_vec_);
            break;
        }
        
        case MessageIDTypes::CHANGE_ANIMATION_1: {

            camera_system_->SetTarget(CORE->GetManager<EntityManager>()->GetPlayerEntities()[0]);
            camera_system_->ToggleTargeted();
            SetAnimation(player_renderer->second, "Player_Walk");

            break;
        }
        
        case MessageIDTypes::CHANGE_ANIMATION_2: {

            SetAnimation(player_renderer->second, "Player_Idle");

            break;
        }

        case MessageIDTypes::FLIP_SPRITE_X: {

            FlipTextureX(dynamic_cast<IWorldObjectRenderer*>(player_renderer->second));
            camera_system_->TempCameraZoom(0.9f);
            break;
        }

        case MessageIDTypes::FLIP_SPRITE_Y: {

            FlipTextureY(dynamic_cast<IWorldObjectRenderer*>(player_renderer->second));
            camera_system_->TempCameraZoom(1.1f);
            break;
        }

        default: {

            break;
        }
    }
}

void GraphicsSystem::AddTextRendererComponent(EntityID id, TextRenderer* text_renderer)
{
    M_DEBUG->WriteDebugMessage("Adding Renderer Component to entity: "
        + std::to_string(id) + "\n");

    //text_renderer_arr_[id] = text_renderer;
    text_renderer_arr_->AddComponent(id, text_renderer);

    if (text_renderer->ui_text_)
    {
        uitext_renderers_in_order_.insert({ GetLayer(text_renderer), text_renderer });
    }

    else
    {
        worldtext_renderers_in_order_.insert({ GetLayer(text_renderer), text_renderer });
    }
}

void GraphicsSystem::RemoveTextRendererComponent(EntityID id)
{
    //TextRendererIt it = text_renderer_arr_.find(id);
    TextRenderer* it = text_renderer_arr_->GetComponent(id);

    int layer;

    if (it) {

        layer = it->layer_;
    	
        if (it->ui_text_)
        {
            TextRenderOrderIt orderit = uitext_renderers_in_order_.find(layer);

            if (orderit != uitext_renderers_in_order_.end()) {

                for ( ; orderit != uitext_renderers_in_order_.end() && (*orderit).first == layer ; ++orderit) {

                    if ((*orderit).second->GetOwner()->GetID() == id) {

                        orderit = uitext_renderers_in_order_.erase(orderit);
                        break;
                    }
                }
            }
        }

        else
        {
            TextRenderOrderIt orderit = worldtext_renderers_in_order_.find(layer);

            if (orderit != worldtext_renderers_in_order_.end()) {

                for (; orderit != worldtext_renderers_in_order_.end() && (*orderit).first == layer; ++orderit) {

                    if ((*orderit).second->GetOwner()->GetID() == id) {

                        orderit = worldtext_renderers_in_order_.erase(orderit);
                        break;
                    }
                }
            }
        }

        M_DEBUG->WriteDebugMessage("Removing Renderer Component from entity: " + std::to_string(id) + "\n");
        layer = GetLayer(it);
        text_renderer_arr_->RemoveComponent(id);
    }
}

void GraphicsSystem::AddTextureRendererComponent(EntityID id, TextureRenderer* texture_renderer) {

    M_DEBUG->WriteDebugMessage("Adding Renderer Component to entity: " + std::to_string(id) + "\n");

    //texture_renderer_arr_[id] = texture_renderer;
    texture_renderer_arr_->AddComponent(id, texture_renderer);
    worldobj_renderers_in_order_.insert({GetLayer(texture_renderer), texture_renderer });
}

void GraphicsSystem::RemoveTextureRendererComponent(EntityID id) {

    //TextureRendererIt it = texture_renderer_arr_.find(id);
    TextureRenderer* it = texture_renderer_arr_->GetComponent(id);
    int layer;

    if (it) {

        M_DEBUG->WriteDebugMessage("Removing Renderer Component from entity: "
            + std::to_string(id) + "\n");

        layer = GetLayer(it);
        texture_renderer_arr_->RemoveComponent(id);
    }

    WorldRenderOrderIt orderit = worldobj_renderers_in_order_.find(layer);

    if (orderit != worldobj_renderers_in_order_.end()) {

        for ( ; orderit != worldobj_renderers_in_order_.end() && (*orderit).first == layer ; ++orderit) {

            if ((*orderit).second->GetOwner()->GetID() == id) {

                orderit = worldobj_renderers_in_order_.erase(orderit);
                break;
            }
        }
    }
}

void GraphicsSystem::AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer) {

    M_DEBUG->WriteDebugMessage("Adding Animation Renderer Component to entity: "
        + std::to_string(id) + "\n");

    anim_renderer_arr_->AddComponent(id, animation_renderer);
    worldobj_renderers_in_order_.insert({GetLayer(animation_renderer), animation_renderer});
}

void GraphicsSystem::RemoveAnimationRendererComponent(EntityID id) {

    AnimationRenderer* it = anim_renderer_arr_->GetComponent(id);
    int layer;

    if (it) {

        M_DEBUG->WriteDebugMessage("Removing Animation Renderer Component from entity: "
            + std::to_string(id) + "\n");

        layer = GetLayer(it);
        anim_renderer_arr_->RemoveComponent(id);
    }

    WorldRenderOrderIt orderit = worldobj_renderers_in_order_.find(layer);

    if (orderit != worldobj_renderers_in_order_.end()) {

        for (; orderit != worldobj_renderers_in_order_.end() && (*orderit).first == layer; ++orderit) {

            if ((*orderit).second->GetOwner()->GetID() == id) {

                orderit = worldobj_renderers_in_order_.erase(orderit);
                break;
            }
        }
    }
}

void GraphicsSystem::UpdateAnimationFrame(AnimationRenderer* anim_renderer, float frametime) {

    if (anim_renderer->play_animation_ == true) {

        anim_renderer->time_elapsed_ += frametime;
        anim_renderer->has_finished_animating_ = false;

        if (anim_renderer->time_elapsed_ >= anim_renderer->current_animation_->GetFrameDuration()) {

            SetToNextFrame(anim_renderer);
            anim_renderer->total_time_elapsed_ += anim_renderer->time_elapsed_;

            if (IsLastFrame(anim_renderer)) {

                anim_renderer->has_finished_animating_ = true;
                anim_renderer->total_time_elapsed_ = 0.0f;
            }

            anim_renderer->time_elapsed_ = 0.0f;
        }
    }
}

void GraphicsSystem::BatchWorldObject(IWorldObjectRenderer* i_worldobj_renderer) {
    
    Vector2D scale =
        component_manager_->GetComponent<Scale>(i_worldobj_renderer->GetOwner()->GetID())->GetScale();
    Transform* transform =
        component_manager_->GetComponent<Transform>(i_worldobj_renderer->GetOwner()->GetID());

    float orientation = static_cast<float>(transform->rotation_ * M_PI / 180);
    Vector2D pos = transform->position_;

    const float global_scale = CORE->GetGlobalScale();

    //glm::vec2 scaling{ scale->GetScale().x, scale->GetScale().y };
    glm::vec2 scaling{ scale.x, scale.y };
    glm::vec2 rotation{ glm::cos(orientation), glm::sin(orientation) };
    glm::vec2 position{ pos.x * global_scale, pos.y * global_scale };

    if (texture_handles.find(*i_worldobj_renderer->texture_handle_) == texture_handles.end())
    {
        texture_handles[*i_worldobj_renderer->texture_handle_] = static_cast<GLuint>(texture_handles.size());
    }

    GLuint tex_id = texture_handles[*i_worldobj_renderer->texture_handle_];

    for (int i = 0; i < 4; ++i)
    {
        tex_vtx_sent.push_back(i_worldobj_renderer->tex_vtx_[i]);
        scaling_sent.push_back(scaling);
        rotation_sent.push_back(rotation);
        position_sent.push_back(position);
        texture_id_sent.push_back(static_cast<float>(tex_id));
    }
}

void GraphicsSystem::DrawBatch(GLuint vbo_hdl, glm::mat3 world_to_ndc_xform)
{
    for (auto tex_it = texture_handles.begin(); tex_it != texture_handles.end(); ++tex_it)
    {
        glBindTextureUnit(tex_it->second, tex_it->first);
    }

    size_t offset = sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * tex_vtx_sent.size(),
                         tex_vtx_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * scaling_sent.size(),
                         scaling_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * rotation_sent.size(),
                         rotation_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * position_sent.size(),
                         position_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(float) * texture_id_sent.size(),
                         texture_id_sent.data());

    int texture_samplers[20]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

    auto loc = glGetUniformLocation(graphic_shaders_["ObjectShader"]->GetHandle(), "uTex2d");
    glUniform1iv(loc, 20, texture_samplers);

    graphic_shaders_["ObjectShader"]->SetUniform("world_to_ndc_xform", world_to_ndc_xform);

    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(6 * tex_vtx_sent.size() / 4 - 2), GL_UNSIGNED_SHORT, NULL);

    tex_vtx_sent.clear();
    scaling_sent.clear();
    rotation_sent.clear();
    position_sent.clear();
    texture_id_sent.clear();
    texture_handles.clear();
}

void GraphicsSystem::DrawTextObject(Shader* shader, Model* model, TextRenderer* text_renderer, glm::vec2 cam_pos) {

    shader->SetUniform("uTex2d", 0);
    shader->SetUniform("projection", projection);
    shader->SetUniform("text_color", text_renderer->color_);

    //Vector2D obj_pos_ = std::dynamic_pointer_cast<Transform>(
    //    text_renderer->GetOwner()->GetComponent(ComponentTypes::TRANSFORM))->position_;
	
    Transform* xform = component_manager_->GetComponent<Transform>(text_renderer->GetOwner()->GetID());
	
    if (!xform)
        return;

    Vector2D obj_pos_ = xform->position_;

    Vector2D pos;
    float scale;

    if (text_renderer->ui_text_)
    {
        pos = obj_pos_;
        scale = text_renderer->scale_;
    }

    else
    {
        glm::vec2 translation{ cam_pos * camera_system_->cam_zoom_ + 0.5f * win_size_ };
        pos = obj_pos_ + Vector2D{ translation.x, translation.y };
        scale = text_renderer->scale_ * camera_system_->cam_zoom_;
    }

    std::string::const_iterator c;
    for (c = text_renderer->text_.begin(); c != text_renderer->text_.end(); c++)
    {
        Character ch = (*text_renderer->font_->GetCharacters())[*c];

        glm::ivec2 bearing = ch.GetBearing();
        glm::ivec2 size = ch.GetSize();

        float xpos = pos.x + bearing.x * scale;
        float ypos = pos.y - (size.y - bearing.y) * scale;

        float w = size.x * scale;
        float h = size.y * scale;
        
        std::vector<glm::vec2> vertices;

        vertices.push_back({ xpos,     ypos + h });
        vertices.push_back({ xpos + w, ypos + h });
        vertices.push_back({ xpos,     ypos });
        vertices.push_back({ xpos + w, ypos });

        // render glyph texture over quad
        glBindTextureUnit(0, ch.GetTexID());

        glNamedBufferSubData(model->GetVBOHandle(), 0,
                             sizeof(glm::vec2) * vertices.size(), vertices.data());
        // render quad
        glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
        
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        pos.x += (ch.GetAdvance() >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void GraphicsSystem::FlipTextureX(IWorldObjectRenderer* i_worldobj_renderer) {

    i_worldobj_renderer->x_mirror_ = !i_worldobj_renderer->x_mirror_;

    std::swap(i_worldobj_renderer->tex_vtx_[0], i_worldobj_renderer->tex_vtx_[2]);
    std::swap(i_worldobj_renderer->tex_vtx_[1], i_worldobj_renderer->tex_vtx_[3]);
}

void GraphicsSystem::FlipTextureY(IWorldObjectRenderer* i_worldobj_renderer) {

    i_worldobj_renderer->y_mirror_ = !i_worldobj_renderer->y_mirror_;

    std::swap(i_worldobj_renderer->tex_vtx_[0], i_worldobj_renderer->tex_vtx_[1]);
    std::swap(i_worldobj_renderer->tex_vtx_[2], i_worldobj_renderer->tex_vtx_[3]);
}

int GraphicsSystem::GetLayer(IWorldObjectRenderer* i_worldobj_renderer) {

    return i_worldobj_renderer->layer_;
}

int GraphicsSystem::GetLayer(TextRenderer* text_renderer)
{
    return text_renderer->layer_;
}

void GraphicsSystem::ChangeTexture(TextureRenderer* renderer, std::string texture_name) {

    renderer->texture_ = *(texture_manager_->GetTexture(texture_name));
    renderer->texture_name_ = texture_name;
}

void GraphicsSystem::AddAnimation(AnimationRenderer* anim_renderer, std::string animation_name) {

    anim_renderer->obj_animations_[animation_name] = animation_manager_->GetAnimation(animation_name);
}

void GraphicsSystem::SetAnimation(AnimationRenderer* anim_renderer, std::string animation_name) {

    anim_renderer->current_animation_ = &anim_renderer->obj_animations_[animation_name];
    anim_renderer->current_animation_name_ = animation_name;
    anim_renderer->time_elapsed_ = 0.0f;

    anim_renderer->texture_handle_ = anim_renderer->current_animation_->GetAnimationFramesHandle();
    anim_renderer->tex_vtx_ = *anim_renderer->current_animation_->GetTexVtx();

    if (anim_renderer->x_mirror_)
    {
        std::swap(anim_renderer->tex_vtx_[0], anim_renderer->tex_vtx_[2]);
        std::swap(anim_renderer->tex_vtx_[1], anim_renderer->tex_vtx_[3]);
    }

    if (anim_renderer->y_mirror_)
    {
        std::swap(anim_renderer->tex_vtx_[0], anim_renderer->tex_vtx_[1]);
        std::swap(anim_renderer->tex_vtx_[2], anim_renderer->tex_vtx_[3]);
    }
}

void GraphicsSystem::SetToNextFrame(AnimationRenderer* anim_renderer) {

    for (int i = 0; i < anim_renderer->tex_vtx_.size(); ++i) {

        anim_renderer->tex_vtx_[i].x +=
            anim_renderer->current_animation_->GetOffsetX() * ( 1 - 2 * anim_renderer->y_mirror_);
    }
}

bool GraphicsSystem::IsLastFrame(AnimationRenderer* anim_renderer) {

    return anim_renderer->total_time_elapsed_ >=
                anim_renderer->current_animation_->GetFrameDuration() *
                anim_renderer->current_animation_->GetNumFrames();
}

GLuint GraphicsSystem::GetFramebuffer()
{
    return final_texture_;
}
