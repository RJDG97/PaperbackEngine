/*                                                                   includes
----------------------------------------------------------------------------- */
#define _USE_MATH_DEFINES

#include "GraphicsSystem.h"
#include "glhelper.h"
#include <FreeImage.h>
#include <cmath>
#include <string>
#include <set>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
GraphicsSystem* GraphicsSystem::graphicsSystem = nullptr;

std::map<unsigned int, GLuint> textures;
std::vector<GraphicsSystem::Model> GraphicsSystem::models;


GraphicsSystem* GraphicsSystem::Instance()
{
    if (!graphicsSystem)
        graphicsSystem = new GraphicsSystem;
    return graphicsSystem;
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
    GLint w{ GLHelper::Instance()->width }, h{ GLHelper::Instance()->height }; // THIS PART IS HARDCODED FOR NOW!!!
    glViewport(0, 0, w, h);

    // Part 3: create different geometries and insert them into
    // repository container GraphicsSystem::models ...

    GraphicsSystem::models.push_back(
        GraphicsSystem::TristripsModel(1, 1, "Shaders/default.vert",
            "Shaders/default.frag"));

    //Initialize FreeImage
    FreeImage_Initialise();
    std::cout << "FreeImage Version " << FreeImage_GetVersion() << std::endl;

    //load textures
    LoadTexture("Resources\\Sprites\\tiles.png", 0);
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::update

@param double delta_time

@return void

This also updates the size of the squares to be rendered in one of the tasks.
*/
void GraphicsSystem::Update(double delta_time) {

}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GraphicsSystem::Draw() {

    //clear back buffer as before...
    glClear(GL_COLOR_BUFFER_BIT);

    GraphicsSystem::models[0].Draw();

    glfwSwapBuffers(GLHelper::Instance()->ptr_window);

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

bool GraphicsSystem::LoadTexture(const char* filename, const unsigned int texID)
{
    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //pointer to the image, once loaded
    FIBITMAP* dib(0);
    //OpenGL's image ID to map to
    GLuint texobj_hdl;

    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(filename, 0);
    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filename);
    //if still unkown, return failure
    if (fif == FIF_UNKNOWN)
    {
        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif))
    {
        dib = FreeImage_Load(fif, filename, PNG_DEFAULT);

        if (FreeImage_GetBPP(dib) != 32)
        {
            dib = FreeImage_ConvertTo32Bits(FreeImage_Load(fif, filename, PNG_DEFAULT));
        }
    }

    //if the image failed to load, return failure
    if (!dib)
    {
        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    //get the image width and height
    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);

    //retrieve the image data
    BYTE* bits = FreeImage_GetBits(dib);
    BYTE* pixels = new BYTE[width * height * 4];

    //if this somehow one of these failed (they shouldn't), return failure
    if ((bits == 0) || (width == 0) || (height == 0))
    {
        std::cout << "Failed to load image " << filename << std::endl;
        return false;
    }

    for (int i = 0; i < width * height; i++)
    {
        pixels[i * 4 + 0] = bits[i * 4 + 2];
        pixels[i * 4 + 1] = bits[i * 4 + 1];
        pixels[i * 4 + 2] = bits[i * 4 + 0];
        pixels[i * 4 + 3] = bits[i * 4 + 3];
    }

    //if this texture ID is in use, unload the current texture
    if (textures.find(texID) != textures.end())
    {
        glDeleteTextures(1, &(textures[texID]));
    }

    glGenTextures(1, &texobj_hdl);
    glBindTexture(GL_TEXTURE_2D, texobj_hdl);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    textures[texID] = texobj_hdl;

    //Free FreeImage's copy of the data
    FreeImage_Unload(dib);

    //return success
    return true;
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
void GraphicsSystem::Model::Draw()
{
    // there are many shader programs initialized - here we're saying
    // which specific shader program should be used to render geometry
    shdr_pgm.Use();
    
    glBindVertexArray(vaoid);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBindTextureUnit(6, textures[0]);
    glTextureParameteri(textures[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(textures[0], GL_TEXTURE_WRAP_T, GL_REPEAT);
    glUseProgram(shdr_pgm.GetHandle());
    GLuint tex_loc = glGetUniformLocation(shdr_pgm.GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 6);

    glDrawElements(GL_TRIANGLE_STRIP, draw_cnt, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    shdr_pgm.UnUse(); 
}
