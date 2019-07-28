
set(FRAMEWORK_SOURCES ${FRAMEWORK_SOURCES}
 ${CMAKE_CURRENT_LIST_DIR}/bindmodules.cpp
 ${CMAKE_CURRENT_LIST_DIR}/global.cpp
 ${CMAKE_CURRENT_LIST_DIR}/global.h
 ${CMAKE_CURRENT_LIST_DIR}/pch.h
 ${CMAKE_CURRENT_LIST_DIR}/core/application.h
 ${CMAKE_CURRENT_LIST_DIR}/core/applicationbase.cpp
 ${CMAKE_CURRENT_LIST_DIR}/core/applicationbase.h
 ${CMAKE_CURRENT_LIST_DIR}/core/applicationconsole.cpp
 ${CMAKE_CURRENT_LIST_DIR}/core/applicationconsole.h
 ${CMAKE_CURRENT_LIST_DIR}/core/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/core/logger.cpp
 ${CMAKE_CURRENT_LIST_DIR}/core/logger.h
 ${CMAKE_CURRENT_LIST_DIR}/core/signal.h
 ${CMAKE_CURRENT_LIST_DIR}/core/traceinfo.h
 ${CMAKE_CURRENT_LIST_DIR}/crypt/crypt.cpp
 ${CMAKE_CURRENT_LIST_DIR}/crypt/crypt.h
 ${CMAKE_CURRENT_LIST_DIR}/crypt/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/crypt/rsa.cpp
 ${CMAKE_CURRENT_LIST_DIR}/crypt/rsa.h
 ${CMAKE_CURRENT_LIST_DIR}/database/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/database/mainmanager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/database/mainmanager.h
 ${CMAKE_CURRENT_LIST_DIR}/database/mariadbmanager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/database/mariadbmanager.h
 ${CMAKE_CURRENT_LIST_DIR}/database/mysqlmanager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/database/mysqlmanager.h
 ${CMAKE_CURRENT_LIST_DIR}/database/sqlitemanager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/database/sqlitemanager.h
 ${CMAKE_CURRENT_LIST_DIR}/database/sqlmanager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/database/sqlmanager.h
 ${CMAKE_CURRENT_LIST_DIR}/net/connection.cpp
 ${CMAKE_CURRENT_LIST_DIR}/net/connection.h
 ${CMAKE_CURRENT_LIST_DIR}/net/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/net/inputmessage.cpp
 ${CMAKE_CURRENT_LIST_DIR}/net/inputmessage.h
 ${CMAKE_CURRENT_LIST_DIR}/net/networkmessage.cpp
 ${CMAKE_CURRENT_LIST_DIR}/net/networkmessage.h
 ${CMAKE_CURRENT_LIST_DIR}/net/outputmessage.cpp
 ${CMAKE_CURRENT_LIST_DIR}/net/outputmessage.h
 ${CMAKE_CURRENT_LIST_DIR}/net/protocol.cpp
 ${CMAKE_CURRENT_LIST_DIR}/net/protocol.h
 ${CMAKE_CURRENT_LIST_DIR}/net/tcpclient.cpp
 ${CMAKE_CURRENT_LIST_DIR}/net/tcpclient.h
 ${CMAKE_CURRENT_LIST_DIR}/net/tcplistener.cpp
 ${CMAKE_CURRENT_LIST_DIR}/net/tcplistener.h
 ${CMAKE_CURRENT_LIST_DIR}/platform/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/platform/platform.h
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/callback.cpp
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/callback.h
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/manager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/manager.h
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/module.cpp
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/module.h
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/signalmanager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/pymodule/signalmanager.h
 ${CMAKE_CURRENT_LIST_DIR}/thread/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/thread/event.cpp
 ${CMAKE_CURRENT_LIST_DIR}/thread/event.h
 ${CMAKE_CURRENT_LIST_DIR}/thread/eventmanager.cpp
 ${CMAKE_CURRENT_LIST_DIR}/thread/eventmanager.h
 ${CMAKE_CURRENT_LIST_DIR}/thread/eventpoll.cpp
 ${CMAKE_CURRENT_LIST_DIR}/thread/eventpoll.h
 ${CMAKE_CURRENT_LIST_DIR}/thread/lookafter.h
 ${CMAKE_CURRENT_LIST_DIR}/time/clock.cpp
 ${CMAKE_CURRENT_LIST_DIR}/time/clock.h
 ${CMAKE_CURRENT_LIST_DIR}/time/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/time/stopper.cpp
 ${CMAKE_CURRENT_LIST_DIR}/time/stopper.h
 ${CMAKE_CURRENT_LIST_DIR}/time/time.cpp
 ${CMAKE_CURRENT_LIST_DIR}/time/time.h
 ${CMAKE_CURRENT_LIST_DIR}/time/timer.cpp
 ${CMAKE_CURRENT_LIST_DIR}/time/timer.h
 ${CMAKE_CURRENT_LIST_DIR}/util/declarations.h
 ${CMAKE_CURRENT_LIST_DIR}/util/file.cpp
 ${CMAKE_CURRENT_LIST_DIR}/util/file.h
 ${CMAKE_CURRENT_LIST_DIR}/util/math.cpp
 ${CMAKE_CURRENT_LIST_DIR}/util/math.h
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
set(FRAMEWORK_SOURCES ${FRAMEWORK_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/platform/windows/platform.cpp)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
set(FRAMEWORK_SOURCES ${FRAMEWORK_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/platform/linux/platform.cpp)
endif()


if(CMAKE_SYSTEM_NAME STREQUAL "Android")
set(FRAMEWORK_SOURCES ${FRAMEWORK_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/platform/android/platform.cpp)
endif()


if(GRAPHICAL_APPLICATION STREQUAL "True")
 set(FRAMEWORK_SOURCES ${FRAMEWORK_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/core/applicationgraphical.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/applicationgraphical.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/color.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/declarations.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/fontmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/fontmanager.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/imagemanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/imagemanager.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/point.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/rect.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/rectsides.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/size.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/soundmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/soundmanager.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/texturemanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/texturemanager.h
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multimedia/window.h
  )
endif()
