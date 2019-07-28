#ifndef FW_PCH_H
#define FW_PCH_H

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <set>
#include <sstream>

#include <assert.h>
#include <condition_variable>
#include <exception>
#include <functional>
#include <memory>
#include <thread>

#include <array>
#include <deque>
#include <list>
#include <map>
#include <unordered_map>

#include <string>
#include <vector>

#include <chrono>
#include <cstdio>

#ifndef NDEBUG
#define FW_DEBUG_INSTRUCTIONS(instructions) \
  if (config.debug_enabled) { instructions }
#else  // #ifndef NDEBUG
#define FW_DEBUG_INSTRUCTIONS(instructions)
#endif  // #ifndef NDEBUG

#endif  // #ifndef FW_PCH_H
