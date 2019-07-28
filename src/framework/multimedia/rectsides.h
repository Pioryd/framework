#ifndef FW_MULTIMEDIA_RECTSIDES_H
#define FW_MULTIMEDIA_RECTSIDES_H

// Can't include, becouse current file is included in declarations.
//#include "declarations.h"

#include <SFML/Graphics.hpp>
#include <type_traits>
#include "../pch.h"

namespace FW::MM {
template <typename T = int>
struct RectSides {
  RectSides() { top = left = right = bottom = T(0); }
  RectSides(T value) { top = left = right = bottom = value; }
  T top;
  T left;
  T right;
  T bottom;
};
}  // namespace FW::MM
#endif  // #ifndef FW_MULTIMEDIA_RECTSIDES_H
