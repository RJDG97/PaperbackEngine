/*                                                                   includes
----------------------------------------------------------------------------- */
#define _USE_MATH_DEFINES

#include "GraphicsSystem.h"
#include "LightingSystem.h"
#include "WindowsSystem.h"
#include "Core.h"
#include <glm/gtc/type_ptr.hpp>
#include <FreeImage.h>
#include <cmath>
#include <string>
#include <set>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
TextureManager GraphicsSystem::texture_manager;
AnimationManager GraphicsSystem::animation_manager;

TextureManager& GraphicsSystem::GetTextureManager()
{
    return texture_manager;
}

AnimationManager& GraphicsSystem::GetAnimationManager()
{
    return animation_manager;
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

    // Part 2: split color buffer into four viewports ...
    window_width = CORE->GetSystem<WindowsSystem>("WindowsSystem")->getWinWidth();
    window_height = CORE->GetSystem<WindowsSystem>("WindowsSystem")->getWinHeight();
    glViewport(0, 0, window_width, window_height);

    // Part 3: create different geometries and insert them into
    // repository container GraphicsSystem::models ...
    models.push_back(TristripsModel(1, 1, "Shaders/default.vert", "Shaders/default.frag"));
    models.push_back(TristripsModel(1, 1, "Shaders/lighting.vert", "Shaders/lighting.frag"));

    glEnable(GL_CULL_FACE);

    texture_manager.Init();
    animation_manager.Init();
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::update

@param double delta_time

@return void

This also updates the size of the squares to be rendered in one of the tasks.
*/
void GraphicsSystem::Update(float frametime) {

}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GraphicsSystem::Draw() {

    glViewport(0, 0, window_width, window_height);
    glBindFramebuffer(GL_FRAMEBUFFER, CORE->GetSystem<LightingSystem>("LightingSystem")->GetFrameBuffer());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //will be settled by the components itself
    models[0].Draw(*(texture_manager.GetTexture(TextureName::Rock)));
    
    //lighting system stuff
    glDisable(GL_DEPTH_TEST);
    CORE->GetSystem<LightingSystem>("LightingSystem")->TestDraw();
    
    //render the final texture
    glViewport(0, 0, window_width, window_height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    models[0].Draw(CORE->GetSystem<LightingSystem>("LightingSystem")->GetFinalTexture());
    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    models[0].Draw(CORE->GetSystem<LightingSystem>("LightingSystem")->GetLightingTexture());

    glfwSwapBuffers(CORE->GetSystem<WindowsSystem>("WindowsSystem")->ptr_window);
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

/*  _________________________________________________________________________ */
/*! GraphicsSystem::setup_shdrpgm

@param none

@return void

Loads the shader files and uses the shader program pipeline functionality
implemented in class GLSLShader
*/
void GraphicsSystem::Model::SetupShdrpgm(std::string vtx_shdr, std::string frg_shdr)
{
    std::vector<std::pair<GLenum, std::string>> shdr_files; 
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vtx_shdr));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frg_shdr));
    shdr_pgm.CompileLinkValidate(shdr_files);

    if (GL_FALSE == shdr_pgm.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << std::endl;
        std::cout << shdr_pgm.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::TristripsModel

@param slices
Number of subintervals divided horizontally

@param stacks
Number of subintervals divided vertically

@param vtx_shdr
Vertex shader used to render the points

@param frg_shdr
Fragment shader used to render the points

@return GLModel
Contains information about the model to be rendered

Sets up a model comprising of triangle fans, using AOS.
*/
GraphicsSystem::Model GraphicsSystem::TristripsModel(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr)
{
    // Generates the vertices required to render triangle strips

    int const count{ (stacks + 1) * (slices + 1) };
    std::vector<glm::vec2> pos_vtx(count);
    std::vector<glm::vec3> clr_vtx(count);
    std::vector<glm::vec2> tex_vtx(count);

    float const u{ 2.f / static_cast<float>(slices) };
    float const v{ 2.f / static_cast<float>(stacks) };

    for (int row{ 0 }, index{ 0 }; row <= stacks; ++row)
    {
        for (int col{ 0 }; col <= slices; ++col)
        {
            pos_vtx[index] = glm::vec2(u * static_cast<float>(col) - 1.f, v* static_cast<float>(row) - 1.f);

            // Randomly generate r, g, b values for vertex color attribute
            clr_vtx[index] = glm::vec3{ static_cast<float>(row) / stacks,
                                        static_cast<float>(col) / slices,
                                            1.0 - static_cast<float>(row) / stacks - static_cast<float>(col) / slices };

            tex_vtx[index++] = glm::vec2{ col, row };
        }
    }

    // Generate the triangle strip's index or element list

    std::vector<GLushort> idx_vtx(((slices + 1) * 2 + 2) * stacks - 2);

    for (int row{ 0 }, index{ 0 }; row <= stacks - 1; ++row)
    {
        for (int col{ 0 }; col <= slices; ++col)
        {
            if (index != 0 && col == 0)
            {
                idx_vtx[index++] = (row + 1) * (slices + 1) + col;
            }

            idx_vtx[index++] = (row + 1) * (slices + 1) + col;
            idx_vtx[index++] = row * (slices + 1) + col;
        }

        if (row != stacks - 1)
        {
            idx_vtx[index] = idx_vtx[index - 1];
            ++index;
        }
    }

    // Generate a VAO handle to encapsulate the VBO(s) and state of this triangle mesh 

    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() +
                                  sizeof(glm::vec3) * clr_vtx.size() +
                                  sizeof(glm::vec2) * tex_vtx.size(),
                                        nullptr, GL_DYNAMIC_STORAGE_BIT);

    glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());

    glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(),
                         sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

    glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
                         sizeof(glm::vec2) * tex_vtx.size(), tex_vtx.data());

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);
    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);

    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
    glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 1, 1);

    glEnableVertexArrayAttrib(vao_hdl, 2);
    glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(), sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 2, 2);

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * idx_vtx.size(),
        reinterpret_cast<GLvoid*>(idx_vtx.data()),
        GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GLApp::GLModel

    GraphicsSystem::Model mdl;
    mdl.vaoid = vao_hdl;
    mdl.primitive_type = GL_TRIANGLE_STRIP;
    mdl.SetupShdrpgm(vtx_shdr, frg_shdr);
    mdl.draw_cnt = idx_vtx.size();           // number of vertices
    mdl.primitive_cnt = count;               // number of triangles
    return mdl;
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::GLModel::Draw

@param none

@return void

Renders the model in a viewport.
*/
void GraphicsSystem::Model::Draw(GLuint texID)
{
    // there are many shader programs initialized - here we're saying
    // which specific shader program should be used to render geometry
    shdr_pgm.Use();
    glBindVertexArray(vaoid);
    glBindTexture(GL_TEXTURE_2D, texID);
    glBindTextureUnit(0, texID);
    glUseProgram(shdr_pgm.GetHandle());
    GLuint tex_loc = glGetUniformLocation(shdr_pgm.GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 0);
    glDrawElements(GL_TRIANGLE_STRIP, draw_cnt, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shdr_pgm.UnUse();
}

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
}
