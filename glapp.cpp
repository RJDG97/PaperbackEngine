/*                                                                   includes
----------------------------------------------------------------------------- */
#define _USE_MATH_DEFINES

#include "glapp.h"
#include "WindowsSystem.h"
#include <cmath>
#include <string>
#include <array>
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
static GLApp    g_glapp;
std::vector<GLApp::GLModel> GLApp::models;

static GLint task = 0;
static GLint modulate = 0;
static int alphaBlending = 0;
static GLfloat size = 0.0f;
static double time = 0.0;
static double easing = 0.0;
static double totalTime = 20.0;

bool GLApp::mPressed = false;
bool GLApp::tPressed = false;
bool GLApp::aPressed = false;

/*  _________________________________________________________________________ */
/*! GLApp::init

@param none

@return void

Initializes the OpenGL rendering pipeline, and then prints the OpenGL
Context information.
*/
void GLApp::init() {
    // Part 1: clear colorbuffer with white color ...
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Part 2: split color buffer into four viewports ...
    GLint w{ WindowsSystem::Instance()->getWinWidth() }, h{ WindowsSystem::Instance()->getWinHeight() };
    glViewport(0, 0, w, h);

    // Part 3: create different geometries and insert them into
    // repository container GLApp::models ...

    GLApp::models.push_back(
        GLApp::tristrips_model(1, 1, "../shaders/tutorial-5.vert",
            "../shaders/tutorial-5.frag"));
}

/*  _________________________________________________________________________ */
/*! GLApp::update

@param double delta_time

@return void

When T is pressed, task is changed.
When M is pressed, modulation is turned off/on.
When A is pressed, alpha blending mode is turned off/on.

This also updates the size of the squares to be rendered in one of the tasks.
*/
void GLApp::update(double delta_time) {

    if (tPressed == true)
    {
        task = (task + 1) % 7;

        if (task == 3)
        {
            time = 0.0f;
        }

        tPressed = false;
    }

    if (mPressed == true)
    {
        modulate = (modulate + 1) % 2;
        mPressed = false;
    }

    if (aPressed == true)
    {
        alphaBlending = (alphaBlending + 1) % 2;

        if (alphaBlending)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        else
        {
            glDisable(GL_BLEND);
        }

        aPressed = false;
    }

    if (task == 2)
    {
        time += delta_time;
        time > 40.0 ? time -= 40.0 : time;
        double t = time / totalTime;
        easing = (sin(M_PI * t - M_PI / 2) + 1) / 2;
        size = static_cast<GLfloat>(16.0f + easing * (256.0 - 16.0));
    }
}

/*  _________________________________________________________________________ */
/*! GLApp::draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GLApp::draw() {
    //write window title
   //GLHelper::title.append

    //clear back buffer as before...
    glClear(GL_COLOR_BUFFER_BIT);

    // render rectangular shape from NDC coordinates to viewport
    GLApp::models[0].draw();
}

/*  _________________________________________________________________________ */
/*! GLApp::cleanup

@param none

@return void

Returns allocated resources
*/
void GLApp::cleanup() {
  // empty for now
}

/*  _________________________________________________________________________ */
/*! GLApp::setup_shdrpgm

@param none

@return void

Loads the shader files and uses the shader program pipeline functionality
implemented in class GLSLShader
*/
void GLApp::GLModel::setup_shdrpgm(std::string vtx_shdr, std::string frg_shdr)
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
/*! GLApp::tristrips_model

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
GLApp::GLModel GLApp::tristrips_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr)
{
    // Generates the vertices required to render triangle strips

    struct vertex
    {
        glm::vec2 pos_vtx;
        glm::vec3 clr_vtx;
        glm::vec2 tex_vtx;
    };

    std::array<vertex, 4> vertices;

    int const count{ (stacks + 1) * (slices + 1) };

    float const u{ 2.f / static_cast<float>(slices) };
    float const v{ 2.f / static_cast<float>(stacks) };

    for (int row{ 0 }, index{ 0 }; row <= stacks; ++row)
    {
        for (int col{ 0 }; col <= slices; ++col)
        {
            vertex temp;

            temp.pos_vtx = glm::vec2{ u * static_cast<float>(col) - 1.f, v* static_cast<float>(row) - 1.f };
            
            // Randomly generate r, g, b values for vertex color attribute
            temp.clr_vtx = glm::vec3{static_cast<float>(row)/stacks,
                                        static_cast<float>(col)/slices,
                                            1.0 - static_cast<float>(row) / stacks - static_cast<float>(col) / slices };

            temp.tex_vtx = glm::vec2{ col, row };

            vertices[index++] = temp;
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
    glNamedBufferStorage(vbo_hdl, sizeof(vertices), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
    //glNamedBufferSubData(vbo_hdl, 0, sizeof(vertices), vertices.data());

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(vertex));
    glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);

    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, offsetof(vertex, vertex::clr_vtx), sizeof(vertex));
    glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 1, 1);

    glEnableVertexArrayAttrib(vao_hdl, 2);
    glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl, offsetof(vertex, vertex::tex_vtx), sizeof(vertex));
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
    GLApp::GLModel mdl;
    mdl.vaoid = vao_hdl;
    mdl.primitive_type = GL_TRIANGLE_STRIP;
    mdl.setup_shdrpgm(vtx_shdr, frg_shdr);
    mdl.draw_cnt = idx_vtx.size();           // number of vertices
    mdl.primitive_cnt = count;               // number of triangles
    return mdl;
}

/*  _________________________________________________________________________ */
/*! GLApp::GLModel::draw

@param none

@return void

Renders the model in a viewport.
*/
void GLApp::GLModel::draw()
{
    // there are many shader programs initialized - here we're saying
    // which specific shader program should be used to render geometry
    shdr_pgm.Use();
    
    glBindVertexArray(vaoid);

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

    // copy object's model-to-NDC matrix to vertex shader's uniform variable uModelToNDC
    GLint uniform_var_task =
        glGetUniformLocation(shdr_pgm.GetHandle(), "task");

    if (uniform_var_task >= 0) {
        glUniform1i(uniform_var_task, task);
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    GLint uniform_var_modulate =
        glGetUniformLocation(shdr_pgm.GetHandle(), "modulate");

    if (uniform_var_modulate >= 0) {
        glUniform1i(uniform_var_modulate, modulate);
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    GLint uniform_var_size =
        glGetUniformLocation(shdr_pgm.GetHandle(), "size");

    if (uniform_var_size >= 0) {
        glUniform1f(uniform_var_size, size);
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    glDrawElements(GL_TRIANGLE_STRIP, draw_cnt, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    shdr_pgm.UnUse(); 
}
