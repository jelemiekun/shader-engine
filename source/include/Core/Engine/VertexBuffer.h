#pragma once
#include <glad/glad.h>

class VertexBuffer {
private:
  unsigned int rendererID;

public:
  VertexBuffer(const void *data, GLuint size, GLenum usage = GL_STATIC_DRAW);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
  void SetData(const void *data, GLuint size);
};
