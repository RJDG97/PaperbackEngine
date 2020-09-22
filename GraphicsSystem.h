/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>
#include "glslshader.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "LightingSystem.h"

class GraphicsSystem
{
    // encapsulates state required to render a geometrical model
    struct Model
    {
        GLenum     primitive_type;
        GLuint     primitive_cnt;
        GLuint     vaoid;

        GLuint     draw_cnt;
        GLSLShader shdr_pgm;

        // member functions defined in glapp.cpp
        void SetupShdrpgm(std::string vtx_shdr, std::string frg_shdr);
        void Draw(GLuint texID);
        void DrawLight(glm::vec3 light_color, glm::vec2 light_center,
                       float intensity, float radius);
    };

    Model TristripsModel(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr);

    // handles all the textures
    static TextureManager texture_manager;

    // handles all the animations
    static AnimationManager animation_manager;

    static LightingSystem lighting_system;

    static GraphicsSystem graphics_system;

    //GLint buffer;

public:
    static GraphicsSystem& Instance();
    static TextureManager& GetTextureManager();
    static AnimationManager& GetAnimationManager();
    static LightingSystem& GetLightingSystem();

    void Init();
    void Update(double delta_time);
    void Draw();
    void CleanUp();

    std::vector<Model> models;
};

#endif
