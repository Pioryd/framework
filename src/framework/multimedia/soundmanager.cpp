#include "soundmanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void SoundManager::init() {
  soundsPath_ = G::Application->getAssetsPath();
  soundsPath_.append("sound");
}

SoundBuffer_ptr SoundManager::getSoundBuffer(const std::string& soundName) {
  std::filesystem::path searchSoundPath(soundName);
  searchSoundPath.make_preferred();

  for (auto& [path, soundBuffer] : buffers_)
    if (path.find(searchSoundPath.string()) != std::string::npos)
      return soundBuffer;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Sound: [" + soundName + "] is not loaded.");)
  return nullptr;
}

void SoundManager::loadSounds() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Check if directory [" +
                                            soundsPath_.string() + "] exist.");)

  if (!std::filesystem::exists(soundsPath_)) {
    FW::G::Logger.error("Path [" + soundsPath_.string() + "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(soundsPath_))
    loadSoundBuffer(entry.path());
}

void SoundManager::loadSoundBuffer(const std::filesystem::path& soundPath) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load sound: [" + soundPath.filename().string() + "].");)

  const std::set<std::string> extensions = {".bmp", ".png", ".tga"};
  if (extensions.find(soundPath.extension().string()) == extensions.end()) {
    G::Logger.warning("Unable to load sound. Wrong extension: [" +
                      soundPath.extension().string() +
                      "]. Supported are: .wav, .ogg, .flax");
    return;
  }

  auto soundBuffer = std::make_shared<SoundBuffer>();

  if (!soundBuffer->loadFromFile(soundPath.string())) {
    G::Logger.error("Unable to load sound file. Full name: " +
                    soundPath.string());
    return;
  }

  buffers_[soundPath.string()] = soundBuffer;
}
}  // namespace FW::MM