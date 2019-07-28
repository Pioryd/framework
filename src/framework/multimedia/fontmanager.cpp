#include "fontmanager.h"

#include "../core/application.h"
#include "../core/logger.h"
#include "../global.h"

namespace FW::MM {
void FontManager::init() {
  fontsPath_ = G::Application->getAssetsPath();
  fontsPath_.append("font");
}

Font_ptr FontManager::getFont(const std::string& fontName) {
  std::filesystem::path searchFontPath(fontName);
  searchFontPath.make_preferred();

  for (auto& [path, font] : fonts_)
    if (path.find(searchFontPath.string()) != std::string::npos) return font;

  FW_DEBUG_INSTRUCTIONS(
      FW::G::Logger.debug("Font: [" + fontName + "] is not loaded.");)
  return nullptr;
}

void FontManager::loadFonts() {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug("Check if directory [" +
                                            fontsPath_.string() + "] exist.");)

  if (!std::filesystem::exists(fontsPath_)) {
    FW::G::Logger.error("Path [" + fontsPath_.string() + "] does NOT exist.");
    return;
  }

  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(fontsPath_))
    loadFont(entry.path());
}

void FontManager::loadFont(const std::filesystem::path& fontPath) {
  FW_DEBUG_INSTRUCTIONS(FW::G::Logger.debug(
      "Load font: [" + fontPath.filename().string() + "].");)

  const std::set<std::string> extensions = {
      ".ttf", ".tte", ".dfont", ".otf", ".aat", ".sil", ".pcf", ".fnt",
      ".bdf", ".pfr", ".pfb",   ".pfm", ".afm", ".inf", ".pfa", ".ofm"};
  if (extensions.find(fontPath.extension().string()) == extensions.end()) {
    G::Logger.warning(
        "Unable to load font. Wrong extension: [" +
        fontPath.extension().string() +
        "]. Supported are: TrueType, Type 1, CFF, OpenType, SFNT, "
        "X11 PCF, Windows FNT, BDF, PFR and Type 42.");
    return;
  }

  auto font = std::make_shared<Font>();

  if (!font->loadFromFile(fontPath.string())) {
    G::Logger.error("Unable to load font file. Full name: " +
                    fontPath.string());
    return;
  }

  fonts_[fontPath.string()] = font;
}
}  // namespace FW::MM
