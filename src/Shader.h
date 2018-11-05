#ifndef CELTEX_SHADER_H
#define CELTEX_SHADER_H

#include <string>

#include <GL/glew.h>

#define CELTEX_CELL_UNIFORM "uCells"

struct Shader {
  GLuint id;
  bool good;

  Shader() : id(0), good(false) {}
  ~Shader() {
    if (good) glDeleteProgram(id);
  }

  bool fromStrings(const char *vert, const char *frag);
  bool fromFiles(const char *vertfn, const char *fragfn);

  void use() const {
    glUseProgram(id);
  }
};

#endif