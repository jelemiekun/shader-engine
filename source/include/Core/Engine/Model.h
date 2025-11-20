#pragma once

#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"

class Model {
public:
  std::vector<Texture> textures_loaded;
  std::vector<Mesh> meshes;
  std::string directory;

  // Optionally remove this two, only used for soft body physics
  std::vector<float> flatVertices;
  std::vector<int> flatIndices;

  glm::mat4 transform;
  glm::vec3 ambient;
  float shininess;
  bool gammaCorrection;

  Model(bool gamma = false);
  Model(std::string const &path, bool gamma = false);
  void loadModel(std::string const &path);
  void Draw(Shader &shader);
  void syncSoftBodyVertices(); // Optionally remove this, only used for soft
                               // body physics
  void setPosition(const glm::vec3 &position);
  void setRotation(float angleDegrees, const glm::vec3 &axis);
  void setRotation(const glm::quat &quaternion);
  void setTransform(const glm::mat4 &transform);

private:
  void processNode(aiNode *node, const aiScene *scene,
                   const glm::mat4 &parentTransform);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            std::string typeName,
                                            const aiScene *scene);
};
