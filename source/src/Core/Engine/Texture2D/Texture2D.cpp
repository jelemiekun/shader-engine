#include "Texture2D.h"
#include "glad/glad.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"

Texture2D::Texture2D()
    : rendererID(0), type(TextureType::Texture2D), localBuffer(nullptr),
      width(0), height(0), bpp(0) {}

Texture2D::Texture2D(const std::string &path) : Texture2D() { load2D(path); }

Texture2D::~Texture2D() { glDeleteTextures(1, &rendererID); }

bool Texture2D::load2D(const std::string &path) {
  type = TextureType::Texture2D;

  stbi_set_flip_vertically_on_load(true);

  glGenTextures(1, &rendererID);
  glBindTexture(GL_TEXTURE_2D, rendererID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
  if (localBuffer) {
    spdlog::info("2D Texture loaded: {}", path);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, localBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(localBuffer);
  } else {
    spdlog::warn("Failed to load 2D texture: {}", path);
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  return true;
}

bool Texture2D::loadCubemap(const std::vector<std::string> &faces) {
  type = TextureType::Cubemap;

  glGenTextures(1, &rendererID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);

  stbi_set_flip_vertically_on_load(false); // cubemaps should not flip

  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &bpp, 4);
    if (data) {
      spdlog::info("Cubemap face loaded: {}", faces[i]);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width,
                   height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      spdlog::warn("Failed to load cubemap face: {}", faces[i]);
      return false;
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  return true;
}

void Texture2D::bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  if (type == TextureType::Texture2D)
    glBindTexture(GL_TEXTURE_2D, rendererID);
  else
    glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID);
}

void Texture2D::unbind() const {
  if (type == TextureType::Texture2D)
    glBindTexture(GL_TEXTURE_2D, 0);
  else
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
