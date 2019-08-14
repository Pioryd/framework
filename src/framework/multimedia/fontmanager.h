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

  Font_ptr get_font(const std::string& fontName);

  void load_fonts();

 protected:
  void load_font(const std::filesystem::path& font_path);

 public:
  Config config;

 protected:
  std::map<std::string, Font_ptr> fonts_;
  std::filesystem::path fonts_path_;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_FONTMANAGER_H
