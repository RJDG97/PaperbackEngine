#include "Manager/ModelManager.h"
#include "Systems/Debug.h"

void ModelManager::Init() {

    M_DEBUG->WriteDebugMessage("Model Manager Init\n");
}

void ModelManager::AddTristripsModel(int slices, int stacks, std::string model_name)
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

    std::vector<GLushort> idx_vtx(((slices + 1) * 2 + 2)* stacks - 2);

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

    Model mdl;
    mdl.vaoid_ = vao_hdl;
    mdl.primitive_type_ = GL_TRIANGLE_STRIP;
    mdl.draw_cnt_ = idx_vtx.size();           // number of vertices
    mdl.primitive_cnt_ = count;               // number of triangles
    models_[model_name] = mdl;
}

void ModelManager::AddLinesModel(int slices, int stacks, std::string model_name)
{
    // Sets the position of the start and end of each line in a line model

    int const count{ (slices + 1) * 2 + (stacks + 1) * 2 };
    std::vector<glm::vec2> pos_vtx(count);
    float const u{ 2.f / static_cast<float>(slices) };
    float const v{ 2.f / static_cast<float>(stacks) };

    int index = 0;

    for (int col{ 0 }; col <= slices; ++col)
    {
        float x{ u * static_cast<float>(col) - 1.0f };
        pos_vtx[index++] = glm::vec2(x, -1.0f);
        pos_vtx[index++] = glm::vec2(x, 1.0f);
    }

    for (int row{ 0 }; row <= stacks; ++row)
    {
        float y{ u * static_cast<float>(row) - 1.0f };
        pos_vtx[index++] = glm::vec2(-1.0f, y);
        pos_vtx[index++] = glm::vec2(1.0f, y);
    }

    // Generate a VAO handle to encapsulate the VBO(s)

    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2)* pos_vtx.size(),
        pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);
    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GLApp::GLModel

    Model mdl;
    mdl.vaoid_ = vao_hdl;
    mdl.primitive_type_ = GL_LINES;
    mdl.draw_cnt_ = count;                      // number of vertices
    mdl.primitive_cnt_ = pos_vtx.size() / 2;    // number of GL_LINES
    models_[model_name] = mdl;
}

Model ModelManager::GetModel(std::string model_name)
{
    return models_[model_name];
}