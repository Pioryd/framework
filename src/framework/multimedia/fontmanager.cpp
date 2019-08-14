#include "fontmanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void FontManager::init() {
  fonts_path_ = G::Application->get_assets_path();
  fonts_path_.append("font");
}

Font_ptr FontManager::get_font(const std::string& fontName) {
  std::filesystem::path search_font_path(fontName);
  search_font_path.make_preferred();

  for (auto& [path, font] : fonts_)
    if (path.find(search_font_path.string()) != std::string::npos) return font;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Font: [" + fontName + "] is not loaded.");)
  return nullptr;
}

void FontManager::load_fonts() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Check if directory [" +
                                            fonts_path_.string() + "] exist.");)

  if (!std::filesystem::exists(fonts_path_)) {
    FW::G::Logger.error("Path [" + fonts_path_.string() + "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(fonts_path_))
    load_font(entry.path());
}

void FontManager::load_font(const std::filesystem::path& font_path) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load font: [" + font_path.filename().string() + "].");)

  const std::set<std::string> extensions = {
      ".ttf", ".tte", ".dfont", ".otf", ".aat", ".sil", ".pcf", ".fnt",
      ".bdf", ".pfr", ".pfb",   ".pfm", ".afm", ".inf", ".pfa", ".ofm"};
  if (extensions.find(font_path.extension().string()) == extensions.end()) {
    G::Logger.warning(
        "Unable to load font. Wrong extension: [" +
        font_path.extension().string() +
        "]. Supported are: TrueType, Type 1, CFF, OpenType, SFNT, "
        "X11 PCF, Windows FNT, BDF, PFR and Type 42.");
    return;
  }

  auto font = std::make_shared<Font>();

  if (!font->loadFromFile(font_path.string())) {
    G::Logger.error("Unable to load font file. Full name: " +
                    font_path.string());
    return;
  }

  fonts_[font_path.string()] = font;
}
}  // namespace FW::MM
