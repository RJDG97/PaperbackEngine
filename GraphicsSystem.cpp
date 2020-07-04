/*                                                                   includes
----------------------------------------------------------------------------- */
#define _USE_MATH_DEFINES

#include <GL/glew.h> // for access to OpenGL API declarations 
#include "GraphicsSystem.h"
#include "WindowsSystem.h"
#include <cmath>
#include <string>
#include <array>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
GraphicsSystem graphicsSystem;
std::vector<GraphicsSystem::GLModel> GraphicsSystem::models;

/*  _________________________________________________________________________ */
/*! GraphicsSystem::init

@param none

@return void

Initializes the OpenGL rendering pipeline, and then prints the OpenGL
Context information.
*/
void GraphicsSystem::init() {

    PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC hdc;
    HGLRC hglrc;
    int PixelFormat;

    hdc = GetDC(WindowsSystem::Instance()->getHandle());
    PixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, PixelFormat, &pfd);
    hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
    }

    // Part 1: clear colorbuffer with white color ...
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Part 2: split color buffer into four viewports ...
    GLint w{ WindowsSystem::Instance()->getWinWidth() }, h{ WindowsSystem::Instance()->getWinHeight() };
    glViewport(0, 0, w, h);

    // Part 3: create different geometries and insert them into
    // repository container GraphicsSystem::models ...

    GraphicsSystem::models.push_back(
        GraphicsSystem::tristrips_model(1, 1, "Shaders/default.vert",
            "Shaders/default.frag"));
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::update

@param double delta_time

@return void

This also updates the size of the squares to be rendered in one of the tasks.
*/
void GraphicsSystem::update(double delta_time) {

}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GraphicsSystem::draw() {

    //clear back buffer as before...
    glClear(GL_COLOR_BUFFER_BIT);

    // render rectangular shape from NDC coordinates to viewport
    GraphicsSystem::models[0].draw();
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::cleanup

@param none

@return void

Returns allocated resources
*/
void GraphicsSystem::cleanup() {
  // empty for now
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::setup_shdrpgm

@param none

@return void

Loads the shader files and uses the shader program pipeline functionality
implemented in class GLSLShader
*/
void GraphicsSystem::GLModel::setup_shdrpgm(std::string vtx_shdr, std::string frg_shdr)
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

//IGNORE THIS ONE NEED TO CHANGE TO USE FREEIMAGE
GLuint setup_texobj(std::string pathname)
{
    // remember all our images have width and height of 256 texels and
    // use 32-bit RGBA texel format
    GLint width {256}, height {256}, bytes_per_texel {4};
    char* ptr_texels = new char[width * height * bytes_per_texel];

    std::string path = "../images/" + pathname + ".tex";
    std::ifstream ifs{ path, std::ios::binary };
    ifs.read(ptr_texels, static_cast<int>(static_cast<double>(width) * height * bytes_per_texel));

    GLuint texobj_hdl;
    // define and initialize a handle to texture object that will
    // encapsulate two-dimensional textures
    glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);
    
    // allocate GPU storage for texture image data loaded from file
    glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, width, height);
    // copy image data from client memory to GPU texture buffer memory
    glTextureSubImage2D(texobj_hdl, 0, 0, 0, width, height,
                            GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);
    
    // client memory not required since image is buffered in GPU memory
    delete[] ptr_texels;
    ifs.close();

    return texobj_hdl;
}


/*  _________________________________________________________________________ */
/*! GraphicsSystem::tristrips_model

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
GraphicsSystem::GLModel GraphicsSystem::tristrips_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr)
{
    // Generates the vertices required to render triangle strips

    int const count{ (stacks + 1) * (slices + 1) };
    std::vector<glm::vec2> pos_vtx(count);
    std::vector<glm::vec3> clr_vtx(count);
    //std::vector<glm::vec2> tex_vtx(4);

    float const u{ 2.f / static_cast<float>(slices) };
    float const v{ 2.f / static_cast<float>(stacks) };

    for (int row{ 0 }, index{ 0 }; row <= stacks; ++row)
    {
        for (int col{ 0 }; col <= slices; ++col)
        {
            pos_vtx[index] = glm::vec2(u * static_cast<float>(col) - 1.f, v* static_cast<float>(row) - 1.f);

            // Randomly generate r, g, b values for vertex color attribute
            clr_vtx[index] = glm::vec3(static_cast<GLfloat>(rand() % 255 / 255.0f),
                               static_cast<GLfloat>(rand() % 255 / 255.0f),
                               static_cast<GLfloat>(rand() % 255 / 255.0f));
            
            //tex_vtx[index++] = glm::vec2{ col, row };
        }
    }

    // Generate the triangle strip's index or element list

    std::vector<GLushort> idx_vtx((slices + 1) * 2 + 2 * stacks - 2);

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
                                  sizeof(glm::vec3) * clr_vtx.size(),// +
                                  //sizeof(glm::vec2) * tex_vtx.size(),
                                        nullptr, GL_DYNAMIC_STORAGE_BIT);

    glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());

    glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(),
        sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

    //glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
    //    sizeof(glm::vec2) * tex_vtx.size(), tex_vtx.data());

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

    /*
    glEnableVertexArrayAttrib(vao_hdl, 2);
    glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(), sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 2, 2);
    */

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * idx_vtx.size(),
        reinterpret_cast<GLvoid*>(idx_vtx.data()),
        GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GraphicsSystem::GLModel

    GraphicsSystem::GLModel mdl;
    mdl.vaoid = vao_hdl;
    mdl.primitive_type = GL_TRIANGLE_STRIP;
    mdl.setup_shdrpgm(vtx_shdr, frg_shdr);
    mdl.draw_cnt = idx_vtx.size();           // number of vertices
    mdl.primitive_cnt = count;               // number of triangles
    return mdl;
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::GLModel::draw

@param none

@return void

Renders the model in a viewport.
*/
void GraphicsSystem::GLModel::draw()
{
    // there are many shader programs initialized - here we're saying
    // which specific shader program should be used to render geometry
    shdr_pgm.Use();
    
    glBindVertexArray(vaoid);

    /*
    if (3 <= task && task <= 6)
    {
        GLuint texobj = setup_texobj("duck-rgba-256");
        // suppose texture object is to use texture image unit 6
        glBindTextureUnit(6, texobj);

        // set what happens when sampler accesses textures outside [0, 1]
        switch (task)
        {
            case 3:
            case 4: glTextureParameteri(texobj, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTextureParameteri(texobj, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    break;
            
            case 5: glTextureParameteri(texobj, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                    glTextureParameteri(texobj, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                    break;

            case 6: glTextureParameteri(texobj, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTextureParameteri(texobj, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    break;
        }

        // suppose shdrpgm is the handle to shader program object
        // that will render the rectangular model
        glUseProgram(shdr_pgm.GetHandle());

        // tell fragment shader sampler uTex2d will use texture image unit 6
        GLuint tex_loc = glGetUniformLocation(shdr_pgm.GetHandle(), "uTex2d");
        glUniform1i(tex_loc, 6);
    }
    */

    glDrawElements(GL_TRIANGLE_STRIP, draw_cnt, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    shdr_pgm.UnUse(); 
}
