#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const GLuint *data, GLuint pCount)
    : count(pCount) {
  // ASSERT(sizeof(unsigned int) == sizeof(GLuint))

  glGenBuffers(1, &rendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, pCount * sizeof(GLuint), data,
               GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer() { glDeleteBuffers(1, &rendererID); }

void ElementBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void ElementBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

GLuint ElementBuffer::getCount() const { return count; }
