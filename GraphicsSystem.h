/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>
#include "glslshader.h"

class GraphicsSystem
{
public:

    static GraphicsSystem* graphicsSystem;
    static GraphicsSystem* Instance();

    static void Init();
    static void Update(double delta_time);
    static void Draw();
    static void CleanUp();

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
        void Draw();
    };
    
    static Model TristripsModel(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr);
    static bool LoadTexture(const char* filename, const unsigned int texID);
    
    static std::vector<Model> models;
};

#endif
