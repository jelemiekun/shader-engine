#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *data, GLuint size, GLenum usage) {
  glGenBuffers(1, &rendererID);
  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &rendererID); }

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, rendererID); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::SetData(const void *data, unsigned int size) {
  glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
