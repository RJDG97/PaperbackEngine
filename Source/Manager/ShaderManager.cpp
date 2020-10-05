#include "Manager/ShaderManager.h"
#include "Systems/Debug.h"

void Shader::DeleteShaderProgram() {

    if (pgm_handle > 0) {

        glDeleteProgram(pgm_handle);
    }
}

void Shader::CompileLinkValidate(std::vector<std::pair<GLenum, std::string>> vec) {

    for (auto& elem : vec) {

        if (CompileShaderFromFile(elem.first, elem.second.c_str()) == GL_FALSE) {

            return;
        }
    }
    
    Link();
    Validate();
    
    PrintActiveAttribs();
    PrintActiveUniforms();
}

GLboolean Shader::CompileShaderFromFile(GLenum shader_type, const std::string& file_name) {

    std::ifstream infile(file_name);

    assert((infile.good() == GL_FALSE, "File not found!"));

    if (pgm_handle <= 0) {

        pgm_handle = glCreateProgram();
        assert((0 == pgm_handle, "Cannot create program handle!"));
    }

    std::ifstream shader_file(file_name, std::ifstream::in);
    
    assert((!shader_file.is_open(), "Error opening file!"));

    std::stringstream buffer;
    buffer << shader_file.rdbuf();
    shader_file.close();

    return CompileShaderFromString(shader_type, buffer.str());
}

GLboolean Shader::CompileShaderFromString(GLenum shader_type,
                                          const std::string& shader_src) {

    if (pgm_handle <= 0) {

        pgm_handle = glCreateProgram();
        
        assert((0 == pgm_handle, "Cannot create program handle!"));
    }

    GLuint shader_handle = 0;

    switch (shader_type)
    {
        case VERTEX_SHADER:
            shader_handle = glCreateShader(GL_VERTEX_SHADER);
            break;
        
        case FRAGMENT_SHADER:
            shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        
        case GEOMETRY_SHADER:
            shader_handle = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        
        case TESS_CONTROL_SHADER: shader_handle = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        
        case TESS_EVALUATION_SHADER:
            shader_handle = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        
        default:
            return GL_FALSE;
    }

    // load shader source code into shader object
    GLchar const* shader_code[] = { shader_src.c_str() };
    glShaderSource(shader_handle, 1, shader_code, NULL);

    // compile the shader
    glCompileShader(shader_handle);

    // check compilation status
    GLint comp_result;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &comp_result);

    assert((comp_result == GL_FALSE, "Cannot create program handle!"));

    // attach the shader to the program object
    glAttachShader(pgm_handle, shader_handle);
    return GL_TRUE;
}

void Shader::Link() {

    if (is_linked) {

        return;
    }
    
    assert((0 == pgm_handle, "Cannot create program handle prior to link!"));

    // link the various compiled shaders
    glLinkProgram(pgm_handle);

    // verify the link status
    GLint lnk_status;
    glGetProgramiv(pgm_handle, GL_LINK_STATUS, &lnk_status);

    assert((lnk_status == GL_FALSE, "Failed to link shader program!"));

    is_linked = GL_TRUE;
}

void Shader::Use() {

    assert((!(pgm_handle > 0 && is_linked == GL_TRUE), "Shader program doesn't exist / isn't linked!"));
    glUseProgram(pgm_handle);
}

void Shader::UnUse() {

    glUseProgram(0);
}

void Shader::Validate() {

    assert((pgm_handle <= 0 || is_linked == GL_FALSE,
        "Shader program handle was not created/linked prior to validation!"));

    glValidateProgram(pgm_handle);
    GLint status;
    glGetProgramiv(pgm_handle, GL_VALIDATE_STATUS, &status);

    assert((status == GL_FALSE, "Failed to validate shader program!"));
}

GLuint Shader::GetHandle() const {

    return pgm_handle;
}

GLboolean Shader::IsLinked() const {

    return is_linked;
}

void Shader::SetUniform(GLchar const *name, GLboolean val) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform1i(loc, val);
}

void Shader::SetUniform(GLchar const *name, GLint val) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform1i(loc, val);

}

void Shader::SetUniform(GLchar const *name, GLfloat val) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform1f(loc, val);
}

void Shader::SetUniform(GLchar const *name, GLfloat x, GLfloat y) {
  
    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform2f(loc, x, y);
}

void Shader::SetUniform(GLchar const *name, GLfloat x, GLfloat y, GLfloat z) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform3f(loc, x, y, z);
}

void Shader::SetUniform(GLchar const *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform4f(loc, x, y, z, w);
}

void Shader::SetUniform(GLchar const *name, glm::vec2 &val) {
    
    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform2f(loc, val.x, val.y);
}

void Shader::SetUniform(GLchar const *name, glm::vec3 &val) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform3f(loc, val.x, val.y, val.z);
}

void Shader::SetUniform(GLchar const *name, glm::vec4 &val) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniform4f(loc, val.x, val.y, val.z, val.w);
}

void Shader::SetUniform(GLchar const *name, glm::mat3 &val) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform(GLchar const *name, glm::mat4 &val) {

    GLint loc = glGetUniformLocation(pgm_handle, name);
    assert((!(loc >= 0), "Uniform variable name does not exist!"));
    glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
}

void Shader::PrintActiveAttribs() const {

    GLint max_length, num_attribs;
    glGetProgramiv(pgm_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
    glGetProgramiv(pgm_handle, GL_ACTIVE_ATTRIBUTES, &num_attribs);
    GLchar *pname = new GLchar[max_length];

    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "Index\t|\tName\n";
    std::cout << "----------------------------------------------------------------------\n";
    
    for (GLint i = 0; i < num_attribs; ++i) {

        GLsizei written;
        GLint size;
        GLenum type;
        glGetActiveAttrib(pgm_handle, i, max_length, &written, &size, &type, pname);
        GLint loc = glGetAttribLocation(pgm_handle, pname);
        std::cout << loc << "\t\t" << pname << std::endl;
    }
    
    std::cout << "----------------------------------------------------------------------\n";
    delete[] pname;
}

void Shader::PrintActiveUniforms() const {

    GLint max_length;
    glGetProgramiv(pgm_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_length);
    GLchar *pname = new GLchar[max_length];
    GLint num_uniforms;
    glGetProgramiv(pgm_handle, GL_ACTIVE_UNIFORMS, &num_uniforms);

    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "Location\t|\tName\n";
    std::cout << "----------------------------------------------------------------------\n";

    for (GLint i = 0; i < num_uniforms; ++i) {

        GLsizei written;
        GLint size;
        GLenum type;
        glGetActiveUniform(pgm_handle, i, max_length, &written, &size, &type, pname);
        GLint loc = glGetUniformLocation(pgm_handle, pname);
        std::cout << loc << "\t\t" << pname << std::endl;
    }
  
    std::cout << "----------------------------------------------------------------------\n";
    delete[] pname;
}

void ShaderManager::Init() {

    M_DEBUG->WriteDebugMessage("Shader Manager Init\n");
}

void ShaderManager::AddShdrpgm(std::string vtx_shdr, std::string frg_shdr, std::string shader_type) {

    std::vector<std::pair<GLenum, std::string>> shdr_files {

        std::make_pair(GL_VERTEX_SHADER, vtx_shdr),
        std::make_pair(GL_FRAGMENT_SHADER, frg_shdr)
    };

    Shader shdr_pgm;
    shdr_pgm.CompileLinkValidate(shdr_files);

    assert((shdr_pgm.IsLinked() == GL_FALSE, "Unable to compile / link / validate shader programs!"));

    // add compiled, linked, and validated shader program to
    // std::map container GLApp::shdrpgms
    shaders_[shader_type] = shdr_pgm;
}

Shader* ShaderManager::GetShdrpgm(std::string shader_name) {

    return &shaders_[shader_name];
}