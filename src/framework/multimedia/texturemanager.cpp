#include "texturemanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void TextureManager::init() {
  texturesPath_ = G::Application->getAssetsPath();
  texturesPath_.append("texture");
}

Texture_ptr TextureManager::getTexture(const std::string& textureName) {
  std::filesystem::path searchTexturePath(textureName);
  searchTexturePath.make_preferred();

  for (auto& [path, texture] : textures_)
    if (path.find(searchTexturePath.string()) != std::string::npos)
      return texture;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Texture: [" + textureName + "] is not loaded.");)
  return nullptr;
}

void TextureManager::loadTextures() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Check if directory [" + texturesPath_.string() + "] exist.");)

  if (!std::filesystem::exists(texturesPath_)) {
    FW::G::Logger.error("Path [" + texturesPath_.string() +
                        "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(texturesPath_))
    loadTexture(entry.path());
}

void TextureManager::loadTexture(const std::filesystem::path& texturePath) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load texture: [" + texturePath.filename().string() + "].");)

  const std::set<std::string> extensions = {".bmp", ".png", ".tga", ".jpg",
                                            ".gif", ".psd", ".hdr", ".pic"};
  if (extensions.find(texturePath.extension().string()) == extensions.end()) {
    G::Logger.warning(
        "Unable to load texture. Wrong extension: [" +
        texturePath.extension().string() +
        "]. Supported are: .bmp, .png, .tga, .jpg, .gif, .psd, .hdr and .pic.");
    return;
  }

  auto texture = std::make_shared<Texture>();

  if (!texture->loadFromFile(texturePath.string())) {
    G::Logger.error("Unable to load texture file. Full name: " +
                    texturePath.string());
    return;
  }

  textures_[texturePath.string()] = texture;
}
}  // namespace FW::MM