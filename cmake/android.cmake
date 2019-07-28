add_definitions(-DFW_PLATFORM_ANDROID)

if(BUILD_ARCH)
  if(BUILD_ARCH STREQUAL "x64")

  elseif(BUILD_ARCH STREQUAL "x86")

  else(BUILD_ARCH STREQUAL)
    message(SEND_ERROR "Wrong BUILD_ARCH set. Set one of 'x64', 'x86'.")
  endif(BUILD_ARCH STREQUAL "x64")
else(BUILD_ARCH)
  message(SEND_ERROR "BUILD_ARCH not set. Set one of 'x64', 'x86'.")
endif(BUILD_ARCH)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

else()

endif()