
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "Shader.h"

bool Shader::fromStrings(const char *vert, const char *frag) {
  GLuint vert_id, frag_id;
  GLint log_len;

  good = false;

  vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert, NULL);
  glCompileShader(vert_id);
  
  glGetShaderiv(vert_id, GL_INFO_LOG_LENGTH, &log_len);
  if (log_len > 0) {
    std::string err(log_len, ' ');
    glGetShaderInfoLog(vert_id, log_len, NULL, &err[0]);

    std::cerr << "vertex shader error: " << err << std::endl;
    glDeleteShader(vert_id);

    return false;
  }

  frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag, NULL);
  glCompileShader(frag_id);

  glGetShaderiv(frag_id, GL_INFO_LOG_LENGTH, &log_len);
  if (log_len > 0) {
    std::string err(log_len, ' ');
    glGetShaderInfoLog(frag_id, log_len, NULL, &err[0]);

    std::cerr << "fragment shader error: " << err << std::endl;
    glDeleteShader(frag_id);

    return false;
  }

  id = glCreateProgram();
  glAttachShader(id, vert_id);
  glAttachShader(id, frag_id);
  glLinkProgram(id);

  glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_len);
  if (log_len > 0) {
    std::string err(log_len, ' ');
    glGetProgramInfoLog(id, log_len, NULL, &err[0]);

    std::cerr << "shader linking error: " << err << std::endl;
    glDeleteProgram(id);
    glDeleteShader(frag_id);
    glDeleteShader(vert_id);

    return false;
  }

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  good = true;

  return true;
}

bool Shader::fromFiles(const char *vertfn, const char *fragfn) {
  std::ifstream vertf(vertfn);
  if (!vertf.is_open()) return false;

  std::ifstream fragf(fragfn);
  if (!fragf.is_open()) return false;

  std::stringstream ss;
  ss << vertf.rdbuf();
  std::string vert = ss.str();

  ss.clear();
  ss << fragf.rdbuf();
  std::string frag = ss.str();

  return fromStrings(vert.c_str(), frag.c_str());
}