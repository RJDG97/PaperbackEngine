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

    // Part 1: clear colorbuffer with white color ...
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

    //lighting_system = CORE->GetSystem<LightingSystem>("LightingSystem");
    windows_system_ = CORE->GetSystem<WindowsSystem>();

    // Part 2: split color buffer into four viewports ...
    window_width_ = windows_system_->getWinWidth();
    window_height_ = windows_system_->getWinHeight();
    glViewport(0, 0, window_width_, window_height_);

    CORE->GetManager<ModelManager>()->AddTristripsModel(1, 1, ModelType::BoxModel);
    CORE->GetManager<ShaderManager>()->AddShdrpgm("Shaders/default.vert", "Shaders/default.frag", ShaderType::TextureShader);
    CORE->GetManager<ShaderManager>()->AddShdrpgm("Shaders/lighting.vert", "Shaders/lighting.frag", ShaderType::LightShader);

    //FACTORY->AddComponentCreator("Renderer", new ComponentCreatorType<Renderer>(ComponentTypes::RENDERER));
    FACTORY->AddComponentCreator("AnimationRenderer", new ComponentCreatorType<AnimationRenderer>(ComponentTypes::ANIMATIONRENDERER));

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
            M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Texture animation updated)\n");
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //draws all the renderer components
    for (RendererIt it = renderer_arr_.begin(); it != renderer_arr_.end(); ++it)
    {
        if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
		}
        (*it).second->Draw();
    }

    for (AnimRendererIt it = anim_renderer_arr_.begin(); it != anim_renderer_arr_.end(); ++it)
    {
        if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Rendering entity: " + std::to_string(it->first) + "\n");
		}
        (*it).second->Draw();
    }

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

    glfwSwapBuffers(windows_system_->ptr_window);
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Cleanup

@param none

@return void

Returns allocated resources
*/
void GraphicsSystem::CleanUp() {
  // empty for now
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

/*
void GraphicsSystem::Model::DrawLight(glm::vec3 light_color, glm::vec2 light_center,
                                      float intensity, float radius)
{
    shdr_pgm.Use();
    glBindVertexArray(vaoid);
    
    GLint uniform_var_light_color =
        glGetUniformLocation(shdr_pgm.GetHandle(), "light_color");

    if (uniform_var_light_color >= 0) {
        glUniform3fv(uniform_var_light_color, 1, glm::value_ptr(light_color));
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }
    
    GLint uniform_var_light_center =
        glGetUniformLocation(shdr_pgm.GetHandle(), "light_center");

    if (uniform_var_light_center >= 0) {
        glUniform2fv(uniform_var_light_center, 1, glm::value_ptr(light_center));
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }
    
    GLint uniform_var_intensity =
        glGetUniformLocation(shdr_pgm.GetHandle(), "intensity");

    if (uniform_var_intensity >= 0) {
        glUniform1f(uniform_var_intensity, intensity);
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    GLint uniform_var_radius =
        glGetUniformLocation(shdr_pgm.GetHandle(), "radius");

    if (uniform_var_radius >= 0) {
        glUniform1f(uniform_var_radius, radius);
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    glDrawElements(GL_TRIANGLE_STRIP, draw_cnt, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
    shdr_pgm.UnUse();
}*/
