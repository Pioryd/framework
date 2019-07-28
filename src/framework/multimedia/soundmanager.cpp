#include "soundmanager.h"

namespace FW::MM {
void SoundManager::init() {}

SoundBuffer_ptr SoundManager::getSoundBuffer(const std::string& soundName) {
  return nullptr;
}

void SoundManager::loadSounds() {}

void SoundManager::loadSoundBuffer(const std::filesystem::path& soundPath) {}
}  // namespace FW::MM