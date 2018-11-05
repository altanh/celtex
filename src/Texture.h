#ifndef CELTEX_TEXTURE_H
#define CELTEX_TEXTURE_H

#include <GL/glew.h>

// wrapper to provide init/cleanup, not going to worry too much about encapsulation
struct Texture {
  GLuint id;
  GLsizei width;
  GLsizei height;

  Texture(GLsizei w, GLsizei h) : width(w), height(h) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }

  ~Texture() {
    glDeleteTextures(1, &id);
  }

  Texture(const Texture &other) = delete;
  Texture &operator=(const Texture &other) = delete;

  void bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
  }
};

#endif