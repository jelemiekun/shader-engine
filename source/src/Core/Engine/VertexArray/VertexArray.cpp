#include "VertexArray.h"
#include "VertexBuffer.h"
#include <glad/glad.h>

VertexArray::VertexArray() { glGenVertexArrays(1, &rendererID); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &rendererID); }

void VertexArray::Bind() const { glBindVertexArray(rendererID); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const std::vector<int> &layout) {
  Bind();
  vb.Bind();

  GLuint offset = 0;
  GLuint stride = 0;
  for (int count : layout) {
    stride += count * sizeof(float);
  }

  for (unsigned int i = 0; i < layout.size(); i++) {
    int count = layout[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(
        i, count, GL_FLOAT, GL_FALSE, stride,
        reinterpret_cast<const void *>(static_cast<uintptr_t>(offset)));
    offset += count * sizeof(float);
  }

  vb.Unbind();
  Unbind();
}
