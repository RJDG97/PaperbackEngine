/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLAPP_H
#define GLAPP_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations
#include "glslshader.h"

struct GLApp {

  static bool tPressed;
  static bool mPressed;
  static bool aPressed;

  static void init();
  static void update(double delta_time);
  static void draw();
  static void cleanup();

  // encapsulates state required to render a geometrical model
  struct GLModel {
      GLenum     primitive_type;
      GLuint     primitive_cnt;
      GLuint     vaoid;

      GLuint     draw_cnt;
      GLSLShader shdr_pgm;

      // member functions defined in glapp.cpp
      void setup_shdrpgm(std::string vtx_shdr, std::string frg_shdr);
      void draw();
  };

  static GLModel tristrips_model(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr);

  static std::vector<GLModel> models;
};

#endif /* GLAPP_H */
