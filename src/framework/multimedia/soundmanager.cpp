#include "soundmanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void SoundManager::init() {
  sounds_path_ = G::Application->get_assets_path();
  sounds_path_.append("sound");
}

SoundBuffer_ptr SoundManager::get_sound_buffer(const std::string& sound_name) {
  std::filesystem::path search_sound_path(sound_name);
  search_sound_path.make_preferred();

  for (auto& [path, sound_buffer] : buffers_)
    if (path.find(search_sound_path.string()) != std::string::npos)
      return sound_buffer;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Sound: [" + sound_name + "] is not loaded.");)
  return nullptr;
}

void SoundManager::load_sounds() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Check if directory [" +
                                            sounds_path_.string() + "] exist.");)

  if (!std::filesystem::exists(sounds_path_)) {
    FW::G::Logger.error("Path [" + sounds_path_.string() + "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(sounds_path_))
    load_sound_buffer(entry.path());
}

void SoundManager::load_sound_buffer(const std::filesystem::path& sound_path) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load sound: [" + sound_path.filename().string() + "].");)

  const std::set<std::string> extensions = {".bmp", ".png", ".tga"};
  if (extensions.find(sound_path.extension().string()) == extensions.end()) {
    G::Logger.warning("Unable to load sound. Wrong extension: [" +
                      sound_path.extension().string() +
                      "]. Supported are: .wav, .ogg, .flax");
    return;
  }

  auto sound_buffer = std::make_shared<SoundBuffer>();

  if (!sound_buffer->loadFromFile(sound_path.string())) {
    G::Logger.error("Unable to load sound file. Full name: " +
                    sound_path.string());
    return;
  }

  buffers_[sound_path.string()] = sound_buffer;
}
}  // namespace FW::MM