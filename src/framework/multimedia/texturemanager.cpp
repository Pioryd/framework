#include "texturemanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void TextureManager::init() {
  textures_path_ = G::Application->get_assets_path();
  textures_path_.append("texture");
}

Texture_ptr TextureManager::get_texture(const std::string& texture_name) {
  std::filesystem::path search_texture_path(texture_name);
  search_texture_path.make_preferred();

  for (auto& [path, texture] : textures_)
    if (path.find(search_texture_path.string()) != std::string::npos)
      return texture;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Texture: [" + texture_name + "] is not loaded.");)
  return nullptr;
}

void TextureManager::load_textures() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Check if directory [" + textures_path_.string() + "] exist.");)

  if (!std::filesystem::exists(textures_path_)) {
    FW::G::Logger.error("Path [" + textures_path_.string() +
                        "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(textures_path_))
    load_texture(entry.path());
}

void TextureManager::load_texture(const std::filesystem::path& texture_path) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load texture: [" + texture_path.filename().string() + "].");)

  const std::set<std::string> extensions = {".bmp", ".png", ".tga", ".jpg",
                                            ".gif", ".psd", ".hdr", ".pic"};
  if (extensions.find(texture_path.extension().string()) == extensions.end()) {
    G::Logger.warning(
        "Unable to load texture. Wrong extension: [" +
        texture_path.extension().string() +
        "]. Supported are: .bmp, .png, .tga, .jpg, .gif, .psd, .hdr and .pic.");
    return;
  }

  auto texture = std::make_shared<Texture>();

  if (!texture->loadFromFile(texture_path.string())) {
    G::Logger.error("Unable to load texture file. Full name: " +
                    texture_path.string());
    return;
  }

  textures_[texture_path.string()] = texture;
}
}  // namespace FW::MM