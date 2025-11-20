#pragma once
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Shader.h"

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  glm::mat4 transform;
  Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds,
       std::vector<Texture> texs);
  void Draw(Shader &shader, const glm::mat4 &transform,
            const glm::vec3 &ambient, const float &shininess);

  // Optionally remove this, only used for soft body physics
  void updateVertices(const std::vector<float> &newVertices);

private:
  unsigned int vao, vbo, ebo;
  void setupMesh();
};
