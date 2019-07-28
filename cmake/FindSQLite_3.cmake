# sqlite3_FOUND
# sqlite3_INCLUDE_DIR
# sqlite3_LIBRARIES = (sqlite3_LIBRARY_RELEASE or sqlite3_LIBRARY_DEBUG)
# sqlite3_LIBRARY_RELEASE
# sqlite3_LIBRARY_DEBUG
# sqlite3_DEFINITIONS

if(CMAKE_SYSTEM_NAME)
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows") # Vcpkg only
    find_package(sqlite3 REQUIRED)
    set(sqlite3_LIBRARIES sqlite3)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (sqlite3_INCLUDE_DIR AND sqlite3_LIBRARIES AND 
        sqlite3_LIBRARY_RELEASE AND sqlite3_LIBRARY_DEBUG)
      SET(sqlite3_FIND_QUIETLY  TRUE)
    endif (sqlite3_INCLUDE_DIR AND sqlite3_LIBRARIES 
        AND sqlite3_LIBRARY_RELEASE AND sqlite3_LIBRARY_DEBUG)

    find_package(PkgConfig)
    pkg_check_modules(PC_SQLITE sqlite3)
    set(sqlite3_DEFINITIONS ${PC_sqlite3_CFLAGS_OTHER})

    FIND_PATH(sqlite3_INCLUDE_DIR NAMES sqlite3.h) 
    FIND_LIBRARY(sqlite3_LIBRARY_DEBUG  NAMES sqlite3d sqlite3 
        HINTS /usr/lib/debug/usr/lib/ )
    FIND_LIBRARY(sqlite3_LIBRARY_RELEASE NAMES sqlite3)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      SET(sqlite3_LIBRARIES ${sqlite3_LIBRARY_DEBUG})
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" OR 
        CMAKE_BUILD_TYPE STREQUAL "Release")
      SET(sqlite3_LIBRARIES ${sqlite3_LIBRARY_RELEASE})
    else(CMAKE_BUILD_TYPE STREQUAL)
      message(SEND_ERROR "\
Wrong CMAKE_BUILD_TYPE set. Set one of 'Debug', 'RelWithDebInfo', 'Release'.")
    endif(CMAKE_BUILD_TYPE STREQUAL)

    IF(sqlite3_INCLUDE_DIR AND sqlite3_LIBRARIES)
      SET(sqlite3_FOUND TRUE)
    ENDIF(sqlite3_INCLUDE_DIR AND sqlite3_LIBRARIES)

    include(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(sqlite3 DEFAULT_MSG sqlite3_INCLUDE_DIR sqlite3_LIBRARIES)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    message(SEND_ERROR "\
Unable to find sqlite3. Android platform unsupported at them moment.")
	else(CMAKE_SYSTEM_NAME STREQUAL)
    message(SEND_ERROR "\
Unable to find sqlite3. Unsupported platform: ${CMAKE_SYSTEM_NAME}")
	endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
else(CMAKE_SYSTEM_NAME)
  message(SEND_ERROR "\
Unable to find sqlite3. No plaform set. Set platform to CMAKE_SYSTEM_NAME.")
endif(CMAKE_SYSTEM_NAME)

MARK_AS_ADVANCED(sqlite3_INCLUDE_DIR sqlite3_LIBRARIES)