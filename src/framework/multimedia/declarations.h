#ifndef FW_MULTIMEDIA_DECLARATIONS_H
#define FW_MULTIMEDIA_DECLARATIONS_H

#include "../pch.h"

#include "point.h"
#include "rect.h"
#include "rectsides.h"
#include "size.h"

namespace sf {
class Image;
class Texture;
class Font;
class SoundBuffer;
}  // namespace sf

namespace FW::MM {
class FontManager;
class ImageManager;
class SoundManager;
class TextureManager;
class Window;

typedef sf::Image Image;
typedef sf::Texture Texture;
typedef sf::Font Font;
typedef sf::SoundBuffer SoundBuffer;

typedef std::shared_ptr<Image> Image_ptr;
typedef std::shared_ptr<Texture> Texture_ptr;
typedef std::shared_ptr<Font> Font_ptr;
typedef std::shared_ptr<SoundBuffer> SoundBuffer_ptr;
}  // namespace FW::MM

#endif  // #ifndef FW_MULTIMEDIA_DECLARATIONS_H
