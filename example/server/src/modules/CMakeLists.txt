﻿set(MODULES_SOURCES ${MODULES_SOURCES}
	${CMAKE_CURRENT_LIST_DIR}/fast_script.cpp
	${CMAKE_CURRENT_LIST_DIR}/stats.cpp
	${CMAKE_CURRENT_LIST_DIR}/protocol.cpp)
	
if(GRAPHICAL_APPLICATION STREQUAL "True")
  set(MODULES_SOURCES ${MODULES_SOURCES}
	${CMAKE_CURRENT_LIST_DIR}/app_window.cpp
  )
endif()

