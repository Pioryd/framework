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

  Image_ptr get_image(const std::string& image_name);

  void load_images();

 protected:
  void load_image(const std::filesystem::path& image_path);

 public:
  Config config;

 protected:
  std::map<std::string, Image_ptr> images_;
  std::filesystem::path images_path_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_IMAGEMANAGER_H
