#pragma once

#include <string>
#include <vector>

enum class TextureType { Texture2D, Cubemap };

class Texture2D {
private:
  unsigned int rendererID;
  TextureType type;
  unsigned char *localBuffer;
  int width;
  int height;
  int bpp;

public:
  Texture2D();
  Texture2D(const std::string &path);

  ~Texture2D();

  bool load2D(const std::string &path);
  bool loadCubemap(const std::vector<std::string> &faces);

  void bind(unsigned int slot = 0) const;
  void unbind() const;

  inline int getWidth() const { return width; }
  inline int getHeight() const { return height; }
};
