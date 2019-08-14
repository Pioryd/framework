#include "global.h"

#include "core/application.h"
#include "core/logger.h"
#include "net/manager.h"
#include "crypt/rsa.h"
#include "pymodule/manager.h"
#include "pymodule/signalmanager.h"
#include "thread/eventmanager.h"
#include "database/mainmanager.h"
#include "time/clock.h"

#ifdef FW_DEF_GRAPHICAL_APPLICATION
#include "multimedia/fontmanager.h"
#include "multimedia/soundmanager.h"
#include "multimedia/imagemanager.h"
#include "multimedia/texturemanager.h"
#include "multimedia/window.h"
#endif

namespace FW::G {
FW::Core::Logger Logger("FW::G::Logger");
FW::Time::Clock Clock;
FW::Crypt::Rsa Rsa(1024);
FW::Thread::EventManager EventManager("FW::G::EventManager");
FW::Database::MainManager DB_MainManager;
FW::Net::Manager Net_Manager;
FW::Thread::LookAfter<FW::PyModule::Manager> PyModule_Manager;
FW::Thread::LookAfter<FW::Core::Application> Application;

#ifdef FW_DEF_GRAPHICAL_APPLICATION
FW::MM::SoundManager SoundManager;
FW::MM::FontManager FontManager;
FW::MM::ImageManager ImageManager;
FW::MM::TextureManager TextureManager;
FW::MM::Window Window;
#endif
}  // namespace FW::G
