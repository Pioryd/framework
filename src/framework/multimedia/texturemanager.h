#ifndef FW_MULTIMEDIA_TEXTUREMANAGER_H
#define FW_MULTIMEDIA_TEXTUREMANAGER_H

#include "declarations.h"

#include "../thread/eventmanager.h"

namespace FW::MM {
class TextureManager {
 public:
  struct Config {
    bool debug_enabled = true;
  };

 public:
  void init();

  Texture_ptr getTexture(const std::string& textureName);

  void loadTextures();

 protected:
  void loadTexture(const std::filesystem::path& texturePath);

 public:
  Config config;

 protected:
  std::map<std::string, Texture_ptr> textures_;
  std::filesystem::path texturesPath_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_TEXTUREMANAGER_H
