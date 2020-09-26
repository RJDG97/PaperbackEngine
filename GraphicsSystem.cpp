/*                                                                   includes
----------------------------------------------------------------------------- */
#define _USE_MATH_DEFINES

#include "GraphicsSystem.h"
#include "Core.h"
#include <glm/gtc/type_ptr.hpp>
#include <FreeImage.h>
#include <cmath>
#include <string>
#include <set>
#include "ModelManager.h"
#include "ComponentTypes.h"
#include "ComponentCreator.h"

/*                                                   objects with file scope
----------------------------------------------------------------------------- */

void GraphicsSystem::CameraInit()
{
    cam_pos = glm::vec2{ 0, 0 };
    cam_size = glm::vec2{ 800, 600 };

    view_xform = glm::mat3{ 1 , 0 , 0,
                            0 , 1 , 0,
                            cam_pos.x , cam_pos.y , 1 };

    // compute other matrices ...
    camwin_to_ndc_xform = glm::mat3{ 2 / cam_size.x , 0 , 0,
                                    0 , 2 / cam_size.y , 0,
                                    0 , 0 , 1 };

    world_to_ndc_xform = camwin_to_ndc_xform * view_xform;
}

void GraphicsSystem::CameraUpdate()
{
    view_xform = glm::mat3{ 1 , 0 , 0,
                            0 , 1 , 0,
                            cam_pos.x , cam_pos.y , 1 };

    camwin_to_ndc_xform = glm::mat3{ 2.0f / cam_size.x , 0.0f , 0.0f,
                                     0.0f , 2.0f / cam_size.y , 0.0f,
                                     0.0f , 0.0f , 1.0f };

    world_to_ndc_xform = camwin_to_ndc_xform * view_xform;
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
    windows_system_ = CORE->GetSystem<WindowsSystem>("WindowsSystem");

    // Part 2: split color buffer into four viewports ...
    window_width_ = windows_system_->getWinWidth();
    window_height_ = windows_system_->getWinHeight();
    glViewport(0, 0, window_width_, window_height_);

    MODELMANAGER->AddTristripsModel(1, 1, ModelType::BoxModel);
    SHADERMANAGER->AddShdrpgm("Shaders/default.vert", "Shaders/default.frag", ShaderType::TextureShader);
    SHADERMANAGER->AddShdrpgm("Shaders/lighting.vert", "Shaders/lighting.frag", ShaderType::LightShader);

    FACTORY->AddComponentCreator("Renderer", new ComponentCreatorType<Renderer>(ComponentTypes::RENDERER));

    CameraInit();

    glEnable(GL_CULL_FACE);
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::update

@param double delta_time

@return void

This also updates the size of the squares to be rendered in one of the tasks.
*/
void GraphicsSystem::Update(float frametime) {

    CameraUpdate();

    //updates all the renderer components
    for (RendererIt renderer = renderer_arr_.begin(); renderer != renderer_arr_.end(); ++renderer)
    {
        (*renderer).second.Update(frametime, world_to_ndc_xform);
    }

}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GraphicsSystem::Draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //draws all the renderer components
    for (RendererIt renderer = renderer_arr_.begin(); renderer != renderer_arr_.end(); ++renderer)
    {
        (*renderer).second.Draw();
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
}

void GraphicsSystem::AddRendererComponent(EntityID id, Renderer* renderer)
{
    renderer_arr_[id] = *renderer;
}

void GraphicsSystem::RemoveRendererComponent(EntityID id)
{
    RendererIt it = renderer_arr_.find(id);

    if (it != renderer_arr_.end()) {

        renderer_arr_.erase(it);
    }
}

void GraphicsSystem::TempMoveCamera()
{
    cam_pos -= glm::vec2{ 10.0f, 10.0f };
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
