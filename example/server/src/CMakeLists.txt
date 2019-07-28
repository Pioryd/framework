set(MAIN_SOURCES ${MAIN_SOURCES}
  ${CMAKE_CURRENT_LIST_DIR}/main.cpp   
  )
  
set(SERVER_SOURCES ${SERVER_SOURCES}
	${CMAKE_CURRENT_LIST_DIR}/declarations.h
	${CMAKE_CURRENT_LIST_DIR}/global.cpp
	${CMAKE_CURRENT_LIST_DIR}/global.h
	${CMAKE_CURRENT_LIST_DIR}/protocolapp.cpp
	${CMAKE_CURRENT_LIST_DIR}/protocolapp.h
	${CMAKE_CURRENT_LIST_DIR}/serverapp.cpp
	${CMAKE_CURRENT_LIST_DIR}/serverapp.h
  )
