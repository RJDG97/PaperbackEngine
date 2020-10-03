/*                                                                   includes
----------------------------------------------------------------------------- */
#define _USE_MATH_DEFINES

#include "Systems/GraphicsSystem.h"
#include "Engine/Core.h"
#include <glm/gtc/type_ptr.hpp>
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

/*                                                   objects with file scope
----------------------------------------------------------------------------- */

void GraphicsSystem::CameraInit()
{
    cam_pos_ = glm::vec2{ 0, 0 };
    cam_size_ = glm::vec2{ 800, 600 };

    view_xform_ = glm::mat3{ 1 , 0 , 0,
                            0 , 1 , 0,
                            cam_pos_.x , cam_pos_.y , 1 };

    // compute other matrices ...
    camwin_to_ndc_xform_ = glm::mat3{ 2 / cam_size_.x , 0 , 0,
                                    0 , 2 / cam_size_.y , 0,
                                    0 , 0 , 1 };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

void GraphicsSystem::CameraUpdate()
{
    view_xform_ = glm::mat3{ 1 , 0 , 0,
                            0 , 1 , 0,
                            cam_pos_.x , cam_pos_.y , 1 };

    camwin_to_ndc_xform_ = glm::mat3{ 2.0f / cam_size_.x , 0.0f , 0.0f,
                                     0.0f , 2.0f / cam_size_.y , 0.0f,
                                     0.0f , 0.0f , 1.0f };

    world_to_ndc_xform_ = camwin_to_ndc_xform_ * view_xform_;
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Init

@param none

@return void

Initializes the OpenGL rendering pipeline, and then prints the OpenGL
Context information.
*/
void GraphicsSystem::Init() {

    // Clear colorbuffer with cyan color ...
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

    //lighting_system = CORE->GetSystem<LightingSystem>("LightingSystem");
    windows_system_ = CORE->GetSystem<WindowsSystem>();

    // Set up viewports
    window_width_ = windows_system_->getWinWidth();
    window_height_ = windows_system_->getWinHeight();
    glViewport(0, 0, window_width_, window_height_);

    // Set up frame buffer for rendering all objects to texture
    glGenFramebuffers(1, &frame_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

    glGenTextures(1, &final_texture_);
    glBindTexture(GL_TEXTURE_2D, final_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width_, window_height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, final_texture_, 0);

    glGenRenderbuffers(1, &render_buffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width_, window_height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Set up all models and shaders
    ModelManager* model_manager = CORE->GetManager<ModelManager>();
    ShaderManager* shader_manager = CORE->GetManager<ShaderManager>();

    model_manager->AddTristripsModel(1, 1, "BoxModel");
    model_manager->AddLinesModel(1, 1, "LinesModel");
    shader_manager->AddShdrpgm("Shaders/object.vert", "Shaders/object.frag", "ObjectShader");
    shader_manager->AddShdrpgm("Shaders/final.vert", "Shaders/final.frag", "FinalShader");
    shader_manager->AddShdrpgm("Shaders/lighting.vert", "Shaders/lighting.frag", "LightShader");
    shader_manager->AddShdrpgm("Shaders/debug.vert", "Shaders/debug.frag", "DebugShader");

    FACTORY->AddComponentCreator("Renderer", new ComponentCreatorType<Renderer>(ComponentTypes::RENDERER));
    FACTORY->AddComponentCreator("AnimationRenderer", new ComponentCreatorType<AnimationRenderer>(ComponentTypes::ANIMATIONRENDERER));

    final_model_ = model_manager->GetModel("BoxModel");
    final_shader_ = shader_manager->GetShdrpgm("FinalShader");

    lighting_texture_ = CORE->GetSystem<LightingSystem>()->GetLightingTexture();

    CameraInit();

    glEnable(GL_CULL_FACE);

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

    CameraUpdate();
    
    //updates all the renderer components
    for (RendererIt it = renderer_arr_.begin(); it != renderer_arr_.end(); ++it)
    {
        if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Scale, Rotation & Translation matrix updated)\n");
		}
        (*it).second->Update(frametime, world_to_ndc_xform_);
    }

    for (AnimRendererIt it = anim_renderer_arr_.begin(); it != anim_renderer_arr_.end(); ++it)
    {
        if (debug_) {
            // Log id of entity and it's updated components that are being updated
            M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Scale, Rotation, Translation matrix & Texture animation updated)\n");
        }
        (*it).second->Update(frametime, world_to_ndc_xform_);
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //draws all the renderer components
    for (RenderOrderIt it = renderers_in_order_.begin(); it != renderers_in_order_.end(); ++it)
    {
        if (debug_) {
			// Log id of entity and its updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
		}
        (*it).second->Draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    DrawFinalTexture(final_model_, final_shader_, final_texture_);
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    DrawFinalTexture(final_model_, final_shader_, *lighting_texture_);

    /* all these work btw*/
    /*
    glViewport(0, 0, window_width_, window_height_);
    glBindFramebuffer(GL_FRAMEBUFFER, lighting_system_->GetFrameBuffer());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Draw should be in some component shet so i go see wat it is tmr!!!!
    //aim is make the renderer and light component
    //then do camera transforms
    //then do animation
    //and in tat order
    MODELMANAGER->GetModel(ModelType::RegularModel).Draw(*(TEXTUREMANAGER.GetTexture(TextureName::Rock)));
    
    //lighting system stuff
    glDisable(GL_DEPTH_TEST);
    lighting_system->TestDraw();
    
    //render the final texture
    glViewport(0, 0, window_width, window_height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    models[0].Draw(lighting_system->GetFinalTexture());
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    models[0].Draw(lighting_system->GetLightingTexture());*/

    if (debug_) { debug_ = !debug_; }
}

void GraphicsSystem::DrawFinalTexture(Model& model, Shader& shader, Texture& texture)
{
    shader.Use();
    glBindVertexArray(model.vaoid_);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTextureUnit(0, texture);
    glUseProgram(shader.GetHandle());
    GLuint tex_loc = glGetUniformLocation(shader.GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 0);

    glDrawElements(GL_TRIANGLE_STRIP, model.draw_cnt_, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shader.UnUse();
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

std::string GraphicsSystem::GetName()
{
    return "GraphicsSystem";
}

void GraphicsSystem::SendMessageD(Message* m)
{
    switch(m->message_id_)
    {
        case MessageIDTypes::DEBUG_ALL:
        {
            debug_ = true;
            break;
        }
        default:
        {
            break;
        }
    }
}

void GraphicsSystem::AddRendererComponent(EntityID id, Renderer* renderer)
{
    M_DEBUG->WriteDebugMessage("Adding Renderer Component to entity: " + std::to_string(id) + "\n");

    renderer_arr_[id] = renderer;
    renderers_in_order_.insert({renderer->GetLayer(), renderer});
}

void GraphicsSystem::RemoveRendererComponent(EntityID id)
{
    RendererIt it = renderer_arr_.find(id);

    if (it != renderer_arr_.end()) {

        M_DEBUG->WriteDebugMessage("Removing Renderer Component from entity: " + std::to_string(id) + "\n");
        renderer_arr_.erase(it);
    }
}

void GraphicsSystem::AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer)
{
    M_DEBUG->WriteDebugMessage("Adding Animation Renderer Component to entity: " + std::to_string(id) + "\n");

    anim_renderer_arr_[id] = animation_renderer;
    renderers_in_order_.insert({animation_renderer->GetLayer(), animation_renderer });
}

void GraphicsSystem::RemoveAnimationRendererComponent(EntityID id)
{
    AnimRendererIt it = anim_renderer_arr_.find(id);

    if (it != anim_renderer_arr_.end()) {

        M_DEBUG->WriteDebugMessage("Removing Animation Renderer Component from entity: " + std::to_string(id) + "\n");
        anim_renderer_arr_.erase(it);
    }
}

void GraphicsSystem::TempMoveCamera()
{
    cam_pos_ -= glm::vec2{ 10.0f, 10.0f };
}