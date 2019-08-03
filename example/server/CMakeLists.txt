cmake_minimum_required (VERSION 3.8)

project (Server)

list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS cms)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

#Setup some defines for example needs. In release mode remove these defines.
set(SHOW_CONSOLE True)
set(GRAPHICAL_APPLICATION True)
set(FW_DEF_PATH_WORK ${CMAKE_CURRENT_SOURCE_DIR}/work)
set(FW_DEF_PATH_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/../assets)
set(FW_DEF_PATH_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../modules)

if(FW_DEF_PATH_WORK)
	add_definitions(-DFW_DEF_PATH_WORK=\"${FW_DEF_PATH_WORK}\")
endif()

if(FW_DEF_PATH_ASSETS)
	add_definitions(-DFW_DEF_PATH_ASSETS=\"${FW_DEF_PATH_ASSETS}\")
else()
	add_definitions(-DFW_DEF_PATH_ASSETS="assets")
endif()

if(FW_DEF_PATH_MODULES)
	add_definitions(-DFW_DEF_PATH_MODULES=\"${FW_DEF_PATH_MODULES}\")
else()
	add_definitions(-DFW_DEF_PATH_MODULES="modules")
endif()

if(FW_DEF_PATH_PYTHON_LIB)
	add_definitions(-DFW_DEF_PATH_PYTHON_LIB=\"${FW_DEF_PATH_PYTHON_LIB}\")
endif()

if(GRAPHICAL_APPLICATION STREQUAL "True")
	add_definitions(-DFW_DEF_GRAPHICAL_APPLICATION)
else()
	set(GRAPHICAL_APPLICATION "False")
endif()

if(CMAKE_SYSTEM_NAME)
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	  include(windows)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		include(linux)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
		include(android)
	else()
		message(SEND_ERROR  "\
Wrong CMAKE_SYSTEM_NAME set. Set one of 'Windows', 'Android', 'Linux'.")
	endif()
else()
	message(SEND_ERROR "\
CMAKE_SYSTEM_NAME not set. Set one of 'Windows', 'Android', 'Linux'.")
endif()

include(../../third_party/cms/cpp/cms/CMakeLists.txt)
include(../../src/framework/CMakeLists.txt)
include(../../src/cmsext/CMakeLists.txt)
include(src/CMakeLists.txt)
include(src/core/CMakeLists.txt)
include(src/modules/CMakeLists.txt)

# For <modules/..> and <core/..>
include_directories (..)
# For <framework/..> and <cmsext/..>
include_directories (../../src)
# For <cms/..>
include_directories (../../third_party/cms/cpp)

# ${WIN32_ENABLED} must be betweenn 2 variables, otherwise it will led to error.
add_executable(${PROJECT_NAME} ${WIN32_ENABLED} ${CORE_SOURCES} 
    ${MODULES_SOURCES} ${MAIN_SOURCES} ${FRAMEWORK_SOURCES} 
    ${CMSEXT_SOURCES} ${CMS_SOURCES})

target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES} ${SFML_LIBRARIES} 
		${TGUI_LIBRARIES} ${cryptopp_LIBRARIES}  ${sqlite3_LIBRARIES} 
    ${mariadb_LIBRARIES} ${Boost_LIBRARIES} ${additional_LIBRARIES})

# Print project configurations
include(print_configuration)