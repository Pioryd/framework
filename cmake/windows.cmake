# Visual Studio support only

add_definitions(-DFW_PLATFORM_WINDOWS)

# Get VCPKG installed packages directory
set(VCPKG_INSTALLED_DIR ${CMAKE_TOOLCHAIN_FILE})
string(REPLACE "/vcpkg.cmake" "" VCPKG_INSTALLED_DIR ${VCPKG_INSTALLED_DIR})
set(VCPKG_INSTALLED_DIR "\
${VCPKG_INSTALLED_DIR}/../../installed/${VCPKG_TARGET_TRIPLET}")

# Set link directories
message("Selected build type: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_directories("${VCPKG_INSTALLED_DIR}/debug/lib")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  link_directories("${VCPKG_INSTALLED_DIR}/lib")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  link_directories("${VCPKG_INSTALLED_DIR}/lib")
else()
  message(SEND_ERROR "\
Wrong CMAKE_BUILD_TYPE set. Set one of 'Debug', 'RelWithDebInfo', 'Release'.")
endif()

# Show console or not
if(GRAPHICAL_APPLICATION STREQUAL "True" AND NOT SHOW_CONSOLE STREQUAL "True")
set(WIN32_ENABLED "WIN32")
set(CMAKE_EXE_LINKER_FLAGS "\
${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Boost
find_package(Boost 1.60.0 COMPONENTS date_time system filesystem iostreams 
    REQUIRED)
# SFML
if(GRAPHICAL_APPLICATION STREQUAL "True")
  set(SFML_LIBRARIES sfml-graphics sfml-audio sfml-window)
  find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)
endif()
# TGUI-SGML
if(GRAPHICAL_APPLICATION STREQUAL "True")
  set(TGUI_LIBRARIES tgui)
  find_package(TGUI REQUIRED)
endif()
# Python
find_package(Python 3.7 REQUIRED) # does not support "3.7.3" string
# pybind11
set(PYBIND11_CPP_STANDARD /std:c++latest) # Enables some MSVC C++17 features
find_package(pybind11 REQUIRED)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # Becouse pybind11 force to python37.lib in debug mode.
  set(PYTHON_LIBRARIES python37_d) 
endif()
# cryptopp
find_package(Crypto++ REQUIRED)
# sqlite3
find_package(SQLite_3 REQUIRED)
# mariadb
set(mariadb_LIBRARIES libmariadb)

# addidtional link_libraries
set(additional_LIBRARIES "")

# Set include directories
include_directories ("${VCPKG_INSTALLED_DIR}/include")
include_directories ("${VCPKG_INSTALLED_DIR}/include/Python3.7")
