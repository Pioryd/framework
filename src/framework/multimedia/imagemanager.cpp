#include "imagemanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void ImageManager::init() {
  imagesPath_ = G::Application->getAssetsPath();
  imagesPath_.append("image");
}

Image_ptr ImageManager::getImage(const std::string& imageName) {
  std::filesystem::path searchImagePath(imageName);
  searchImagePath.make_preferred();

  for (auto& [path, image] : images_)
    if (path.find(searchImagePath.string()) != std::string::npos) return image;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Image: [" + imageName + "] is not loaded.");)
  return nullptr;
}

void ImageManager::loadImages() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Check if directory [" +
                                            imagesPath_.string() + "] exist.");)

  if (!std::filesystem::exists(imagesPath_)) {
    FW::G::Logger.error("Path [" + imagesPath_.string() + "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(imagesPath_))
    loadImage(entry.path());
}

void ImageManager::loadImage(const std::filesystem::path& imagePath) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load image: [" + imagePath.filename().string() + "].");)

  const std::set<std::string> extensions = {".bmp", ".png", ".tga", ".jpg",
                                            ".gif", ".psd", ".hdr", ".pic"};
  if (extensions.find(imagePath.extension().string()) == extensions.end()) {
    G::Logger.warning(
        "Unable to load image. Wrong extension: [" +
        imagePath.extension().string() +
        "]. Supported are: .bmp, .png, .tga, .jpg, .gif, .psd, .hdr and .pic.");
    return;
  }

  auto image = std::make_shared<Image>();

  if (!image->loadFromFile(imagePath.string())) {
    G::Logger.error("Unable to load image file. Full name: " +
                    imagePath.string());
    return;
  }

  images_[imagePath.string()] = image;
}
}  // namespace FW::MM