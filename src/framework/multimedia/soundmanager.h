#ifndef FW_MULTIMEDIA_SOUNDMANAGER_H
#define FW_MULTIMEDIA_SOUNDMANAGER_H

#include "declarations.h"

#include <SFML/Audio.hpp>

namespace FW::MM {
class SoundManager {
 public:
  struct Config {
    bool debug_enabled = true;
  };

 public:
  void init();

  SoundBuffer_ptr get_sound_buffer(const std::string& sound_name);

  void load_sounds();

 protected:
  void load_sound_buffer(const std::filesystem::path& sound_path);

 public:
  Config config;

 protected:
  std::map<std::string, SoundBuffer_ptr> buffers_;
  std::filesystem::path sounds_path_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_SOUNDMANAGER_H