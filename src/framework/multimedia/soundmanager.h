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

  SoundBuffer_ptr getSoundBuffer(const std::string& soundName);

  void loadSounds();

 protected:
  void loadSoundBuffer(const std::filesystem::path& soundPath);

 public:
  Config config;

 protected:
  std::map<std::string, SoundBuffer_ptr> buffers_;
  std::filesystem::path soundsPath_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_SOUNDMANAGER_H