# cryptopp_FOUND 
# cryptopp_INCLUDE_DIR
# cryptopp_LIBRARIES

# This bug occure on Windows 10 platform. Not tested on linux.
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")  
  set(CMAKE_BUILD_TYPE "Release")
  message("\
[CMAKE_BUILD_TYPE] changed from [RelWithDebInfo] to [Release] for cryptopp \
needs. All source bellow will use [Release]. If some code need \
[RelWithDebInfo] make sure that is before [find_package(Cryptopp REQUIRED)]")
endif()

if(CMAKE_SYSTEM_NAME)
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows") # Vcpkg only
    find_package(cryptopp REQUIRED)
    set(cryptopp_LIBRARIES cryptopp-static)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_path(cryptopp_INCLUDE_DIR NAMES cryptopp/cryptlib.h)
    find_library(cryptopp_LIBRARIES NAMES cryptopp libcryptopp)

    IF(cryptopp_INCLUDE_DIR AND cryptopp_LIBRARIES)
      SET(cryptopp_FOUND TRUE)
    ENDIF(cryptopp_INCLUDE_DIR AND cryptopp_LIBRARIES)

    include(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Cryptopp DEFAULT_MSG 
        cryptopp_INCLUDE_DIR cryptopp_LIBRARIES)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    message(SEND_ERROR  "\
Unable to find cryptopp. Android platform unsupported at them moment.")
	else()
    message(SEND_ERROR "\
Unable to find cryptopp. Unsupported platform: ${CMAKE_SYSTEM_NAME}")
	endif()
else()
  message(SEND_ERROR "\
Unable to find cryptopp. No plaform set. Set platform to CMAKE_SYSTEM_NAME.")
endif()

mark_as_advanced(cryptopp_INCLUDE_DIR cryptopp_LIBRARIES)