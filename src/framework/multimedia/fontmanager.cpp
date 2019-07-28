#include "fontmanager.h"

namespace FW::MM {
void FontManager::init() {}

Font_ptr FontManager::getFont(const std::string& fontName) {
  return Font_ptr();
}

void FontManager::loadFonts() {}

void FontManager::loadFont(const std::filesystem::path& fontPath) {}
}  // namespace FW::MM
