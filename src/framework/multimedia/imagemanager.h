#ifndef FW_MULTIMEDIA_IMAGEMANAGER_H
#define FW_MULTIMEDIA_IMAGEMANAGER_H

#include "declarations.h"
namespace FW::MM {
class ImageManager {
 public:
  struct Config {
    bool debug_enabled = true;
  };

 public:
  void init();

  Image_ptr getImage(const std::string& imageName);

  void loadImages();

 protected:
  void loadImage(const std::filesystem::path& imagePath);

 public:
  Config config;

 protected:
  std::map<std::string, Image_ptr> images_;
  std::filesystem::path imagesPath_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_IMAGEMANAGER_H
