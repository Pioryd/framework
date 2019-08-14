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

  Texture_ptr get_texture(const std::string& texture_name);

  void load_textures();

 protected:
  void load_texture(const std::filesystem::path& texture_path);

 public:
  Config config;

 protected:
  std::map<std::string, Texture_ptr> textures_;
  std::filesystem::path textures_path_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_TEXTUREMANAGER_H
