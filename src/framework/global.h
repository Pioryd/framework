#ifndef FW_GLOBAL_H
#define FW_GLOBAL_H

#include <boost/current_function.hpp>
#include "core/declarations.h"
#include "crypt/declarations.h"
#include "net/declarations.h"
#include "platform/declarations.h"
#include "pymodule/declarations.h"
#include "thread/declarations.h"
#include "thread/lookafter.h"
#include "database/declarations.h"
#include "time/declarations.h"

#ifdef FW_DEF_GRAPHICAL_APPLICATION
#include "multimedia/declarations.h"
#endif

// To use below macros class must inherith from class TraceInfo
#define FW_G_LOGGER_DEBUG_TRACE(message) \
  FW::G::Logger.debug(message, BOOST_CURRENT_FUNCTION, traceInfo)
#define FW_G_LOGGER_INFO_TRACE(message) \
  FW::G::Logger.info(message, BOOST_CURRENT_FUNCTION, traceInfo)
#define FW_G_LOGGER_WARNING_TRACE(message) \
  FW::G::Logger.warning(message, BOOST_CURRENT_FUNCTION, traceInfo)
#define FW_G_LOGGER_ERROR_TRACE(message) \
  FW::G::Logger.error(message, BOOST_CURRENT_FUNCTION, traceInfo)
#define FW_G_LOGGER_FATAL_TRACE(message) \
  FW::G::Logger.fatal(message, BOOST_CURRENT_FUNCTION, traceInfo)

// Globals used by freamwork and project.
namespace FW::G {
extern FW::Core::Logger Logger;
extern FW::Time::Clock Clock;
extern FW::Crypt::Rsa Rsa;
extern FW::Thread::EventManager EventManager;
extern FW::Database::MainManager DB_MainManager;
extern FW::Net::Manager Net_Manager;
extern FW::Thread::LookAfter<FW::PyModule::Manager> PyModule_Manager;
extern FW::Thread::LookAfter<FW::Core::Application> Application;

#ifdef FW_DEF_GRAPHICAL_APPLICATION
extern FW::MM::SoundManager SoundManager;
extern FW::MM::FontManager FontManager;
extern FW::MM::ImageManager ImageManager;
extern FW::MM::TextureManager TextureManager;
extern FW::MM::Window Window;
#endif
}  // namespace FW::G
#endif  // #ifndef FW_GLOBAL_H
