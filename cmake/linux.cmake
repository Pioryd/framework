add_definitions(-DFW_PLATFORM_LINUX=LINUX)

set(WARNS_FLAGS "\
-Wl,--copy-dt-needed-entries -Wall -Wextra -Wno-unused -Wno-unused-parameter \
-Wno-unused-result")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNS_FLAGS} ${ARCH_FLAGS} \
${CPP2011_FLAGS} -pipe")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_REQUIRED_FLAGS -std=c++17)

message("Selected build type: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O1 -g -fno-omit-frame-pointer")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2")
  set(CMAKE_CXX_FLAGS_PERFORMANCE "-Ofast -march=native")
else()
  message(SEND_ERROR "\
Wrong CMAKE_BUILD_TYPE set. Set one of 'Debug', 'RelWithDebInfo', 'Release'.")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)

# Boost
find_package(Boost 1.60.0 COMPONENTS date_time system filesystem iostreams 
    REQUIRED)
# SFML
if(GRAPHICAL_APPLICATION STREQUAL "True")
  set(SFML_LIBRARIES sfml-graphics sfml-audio sfml-window)
  find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)
endif()
# Python
find_package(PythonLibs 3.7.3 REQUIRED)
# pybind11
set(PYBIND11_CPP_STANDARD -std=c++1z
find_package(pybind11 REQUIRED)
# cryptopp
find_package(Crypto++ REQUIRED)
# sqlite3
find_package(SQLite_3 REQUIRED)
# mariadb
set(mariadb_LIBRARIES mysqlclient)

# addidtional link_libraries
set(additional_LIBRARIES stdc++fs)

# Set include directories
include_directories (src/third_party)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIR})
include_directories(${pybind11_INCLUDE_DIR})
include_directories(${sqlite_INCLUDE_DIR})