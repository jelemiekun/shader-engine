#pragma once
#include <glad/glad.h>

class ElementBuffer {
private:
  unsigned int rendererID;
  unsigned int count;

public:
  ElementBuffer(const GLuint *data, GLuint pCount);
  ~ElementBuffer();

  void Bind() const;
  void Unbind() const;

  inline GLuint getCount() const;
};
