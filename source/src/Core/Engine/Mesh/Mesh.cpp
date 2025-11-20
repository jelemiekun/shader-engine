#include "Mesh.h"
#include "Shader.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <spdlog/spdlog.h>

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds,
           std::vector<Texture> texs)
    : vertices(verts), indices(inds), textures(texs),
      transform(glm::mat4(1.0f)) {
  setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  if (!vertices.empty())
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);
  else
    spdlog::warn("Mesh::setupMesh() - No vertex data found!");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if (!indices.empty())
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);
  else
    spdlog::warn("Mesh::setupMesh() - No index data found!");

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);
  // Normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(1);
  // TexCoords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(2);
  // Tangent
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Tangent));
  glEnableVertexAttribArray(3);
  // Bitangent
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Bitangent));
  glEnableVertexAttribArray(4);

  glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader, const glm::mat4 &transform,
                const glm::vec3 &ambient, const float &shininess) {
  if (indices.empty()) {
    spdlog::warn("Mesh::Draw() - No index data found!");
    return;
  }

  int diffuseNum = 0;
  int specularNum = 0;
  // Binds all the textures to their own texture units and sets the respective
  // uniforms in the fragment shader
  for (int i = 0; i < textures.size(); ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(++diffuseNum);
    else if (name == "texture_specular")
      number = std::to_string(++specularNum);
    shader.setInt("material." + name + number, i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glm::mat4 transformedMesh = transform * this->transform;
  shader.setMat4("u_Model", transformedMesh);
  shader.setVec3("material.ambient", ambient);
  shader.setFloat("material.shininess", shininess);

  // Draws the mesh
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  // Resets the active texture unit
  glActiveTexture(GL_TEXTURE0);
}

// Optionally remove this, only used for soft body physics
void Mesh::updateVertices(const std::vector<float> &newVertices) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(float),
                  newVertices.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
