#pragma once
#include <glad/glad.h>
#include <vector>

class VertexBuffer;

class VertexArray {
private:
  GLuint rendererID;

public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;

  void AddBuffer(const VertexBuffer &vb, const std::vector<int> &layout);
};
