#include "Model.h"
#include "stb_image.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

static unsigned int TextureFromFile(const char *path,
                                    const std::string &directory,
                                    const aiScene *scene, bool gamma = false);
static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from);

Model::Model(std::string const &path, bool gamma)
    : transform(glm::mat4(1.0f)), ambient(glm::vec3(0.2f)), shininess(32),
      gammaCorrection(gamma) {
  loadModel(path);
}

Model::Model(bool gamma)
    : transform(glm::mat4(1.0f)), ambient(glm::vec3(0.2f)), shininess(32),
      gammaCorrection(gamma) {}

void Model::Draw(Shader &shader) {
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].Draw(shader, transform, ambient, shininess);
}

// Optionally remove this, only used for soft body physics
void Model::syncSoftBodyVertices() {
  for (auto &mesh : meshes) {
    mesh.updateVertices(flatVertices);
  }
}

void Model::loadModel(std::string const &path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    spdlog::error("Error: ASSIMP::{}", importer.GetErrorString());
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene, glm::mat4(1.0f));

  spdlog::info("Model loaded successfully: {}", path);
}

void Model::processNode(aiNode *node, const aiScene *scene,
                        const glm::mat4 &parentTransform) {
  glm::mat4 nodeTransform =
      parentTransform * aiMatrix4x4ToGlm(node->mTransformation);

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh processedMesh = processMesh(mesh, scene);
    processedMesh.transform = nodeTransform;
    meshes.push_back(processedMesh);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene, nodeTransform);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  int vertexOffset =
      flatVertices.size() /
      3; // Optionally remove this, only used for soft body physics

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.Position = vector;

    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
    } else {
      vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    if (mesh->mTextureCoords[0]) {
      glm::vec2 texCoord;
      texCoord.x = mesh->mTextureCoords[0][i].x;
      texCoord.y = mesh->mTextureCoords[0][i].y;
      vertex.TexCoords = texCoord;
    } else {
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }

    if (mesh->HasTangentsAndBitangents()) {
      vector.x = mesh->mTangents[i].x;
      vector.y = mesh->mTangents[i].y;
      vector.z = mesh->mTangents[i].z;
      vertex.Tangent = vector;

      vector.x = mesh->mBitangents[i].x;
      vector.y = mesh->mBitangents[i].y;
      vector.z = mesh->mBitangents[i].z;
      vertex.Bitangent = vector;
    } else {
      vertex.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
      vertex.Bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // Optionally remove this, only used for soft body physics
    flatVertices.push_back(vertex.Position.x);
    flatVertices.push_back(vertex.Position.y);
    flatVertices.push_back(vertex.Position.z);

    flatVertices.push_back(vertex.Normal.x);
    flatVertices.push_back(vertex.Normal.y);
    flatVertices.push_back(vertex.Normal.z);

    flatVertices.push_back(vertex.TexCoords.x);
    flatVertices.push_back(vertex.TexCoords.y);

    flatVertices.push_back(vertex.Tangent.x);
    flatVertices.push_back(vertex.Tangent.y);
    flatVertices.push_back(vertex.Tangent.z);

    flatVertices.push_back(vertex.Bitangent.x);
    flatVertices.push_back(vertex.Bitangent.y);
    flatVertices.push_back(vertex.Bitangent.z);
    // End of optionally remove this

    // Not included in optionally removing
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
      flatIndices.push_back(face.mIndices[j] + vertexOffset);
    }
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  std::vector<Texture> diffuseMaps = loadMaterialTextures(
      material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  std::vector<Texture> specularMaps = loadMaterialTextures(
      material, aiTextureType_SPECULAR, "texture_specular", scene);
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  std::vector<Texture> normalMaps = loadMaterialTextures(
      material, aiTextureType_HEIGHT, "texture_normal", scene);
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  std::vector<Texture> heightMaps = loadMaterialTextures(
      material, aiTextureType_AMBIENT, "texture_height", scene);
  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 std::string typeName,
                                                 const aiScene *scene) {
  std::vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    spdlog::info("Texture path: {}", str.C_Str());

    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
      if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      Texture texture;
      texture.id = TextureFromFile(str.C_Str(), this->directory, scene);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  return textures;
}

void Model::setPosition(const glm::vec3 &position) {
  transform[3] = glm::vec4(position, 1.0f);
}

void Model::setRotation(float angleDegrees, const glm::vec3 &axis) {
  glm::vec3 translation = glm::vec3(transform[3]);

  transform = glm::mat4(1.0f);
  transform = glm::rotate(transform, glm::radians(angleDegrees), axis);
  transform[3] = glm::vec4(translation, 1.0f);
}

void Model::setRotation(const glm::quat &quaternion) {
  glm::vec3 translation = glm::vec3(transform[3]);
  transform = glm::mat4_cast(quaternion);
  transform[3] = glm::vec4(translation, 1.0f);
}

void Model::setTransform(const glm::mat4 &transform) {
  this->transform = transform;
}

static unsigned int TextureFromFile(const char *path,
                                    const std::string &directory,
                                    const aiScene *scene, bool gamma) {
  stbi_set_flip_vertically_on_load(true);
  unsigned int textureID;
  glGenTextures(1, &textureID);

  std::string texturePath(path);

  if (texturePath[0] == '*') {
    // Handle embedded texture
    int texIndex = std::stoi(texturePath.substr(1)); // removes '*'
    const aiTexture *tex = scene->mTextures[texIndex];

    if (tex) {
      if (tex->mHeight == 0) {
        // Compressed format like PNG or JPG
        int width, height, nrComponents;
        unsigned char *data = stbi_load_from_memory(
            reinterpret_cast<unsigned char *>(tex->pcData), tex->mWidth, &width,
            &height, &nrComponents, 0);

        if (data) {
          GLenum format = (nrComponents == 1)   ? GL_RED
                          : (nrComponents == 3) ? GL_RGB
                                                : GL_RGBA;

          glBindTexture(GL_TEXTURE_2D, textureID);
          glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                       GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);

          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                          GL_LINEAR_MIPMAP_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

          stbi_image_free(data);
        } else {
          spdlog::error("Failed to load embedded texture from memory: {}",
                        path);
        }
      } else {
        spdlog::warn(
            "Uncompressed embedded texture not supported (height > 0)");
      }
    }
  } else {
    // External texture file
    std::string filename = directory + '/' + path;
    int width, height, nrComponents;
    unsigned char *data =
        stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
      GLenum format = (nrComponents == 1)   ? GL_RED
                      : (nrComponents == 3) ? GL_RGB
                                            : GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    } else {
      spdlog::error("Texture failed to load at path: {}", filename);
      stbi_image_free(data);
    }
  }

  return textureID;
}

static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
  return glm::mat4(from.a1, from.b1, from.c1, from.d1, from.a2, from.b2,
                   from.c2, from.d2, from.a3, from.b3, from.c3, from.d3,
                   from.a4, from.b4, from.c4, from.d4);
}
