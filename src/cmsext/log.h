#ifndef FW_CMSEXT_LOG_H
#define FW_CMSEXT_LOG_H

#include <cmsext/declarations.h>

namespace cmsext::log {
void debug(const cms::string& what);
void info(const cms::string& what);
void warning(const cms::string& what);
void error(const cms::string& what);
void fatal(const cms::string& what);
}  // namespace cmsext::log
#endif  // #ifndef FW_CMSEXT_LOG_H
