#ifndef FW_MULTIMEDIA_FONTMANAGER_H
#define FW_MULTIMEDIA_FONTMANAGER_H

#include "declarations.h"

namespace FW::MM {
class FontManager {
 public:
  struct Config {
    bool debug_enabled = true;
  };

 public:
  void init();

  Font_ptr getFont(const std::string& fontName);

  void loadFonts();

 protected:
  void loadFont(const std::filesystem::path& fontPath);

 public:
  Config config;

 protected:
  std::map<std::string, Font_ptr> fonts_;
  std::filesystem::path fontsPath_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_FONTMANAGER_H
