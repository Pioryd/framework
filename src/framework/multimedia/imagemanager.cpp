#include "imagemanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void ImageManager::init() {
  images_path_ = G::Application->get_assets_path();
  images_path_.append("image");
}

Image_ptr ImageManager::get_image(const std::string& image_name) {
  std::filesystem::path search_image_path(image_name);
  search_image_path.make_preferred();

  for (auto& [path, image] : images_)
    if (path.find(search_image_path.string()) != std::string::npos) return image;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Image: [" + image_name + "] is not loaded.");)
  return nullptr;
}

void ImageManager::load_images() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Check if directory [" +
                                            images_path_.string() + "] exist.");)

  if (!std::filesystem::exists(images_path_)) {
    FW::G::Logger.error("Path [" + images_path_.string() + "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(images_path_))
    load_image(entry.path());
}

void ImageManager::load_image(const std::filesystem::path& image_path) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load image: [" + image_path.filename().string() + "].");)

  const std::set<std::string> extensions = {".bmp", ".png", ".tga", ".jpg",
                                            ".gif", ".psd", ".hdr", ".pic"};
  if (extensions.find(image_path.extension().string()) == extensions.end()) {
    G::Logger.warning(
        "Unable to load image. Wrong extension: [" +
        image_path.extension().string() +
        "]. Supported are: .bmp, .png, .tga, .jpg, .gif, .psd, .hdr and .pic.");
    return;
  }

  auto image = std::make_shared<Image>();

  if (!image->loadFromFile(image_path.string())) {
    G::Logger.error("Unable to load image file. Full name: " +
                    image_path.string());
    return;
  }

  images_[image_path.string()] = image;
}
}  // namespace FW::MM