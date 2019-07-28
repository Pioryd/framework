# Print INCLUDE_DIRECTORIES
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 
  INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "INCLUDE_DIRECTORY='${dir}'")
endforeach()

# Print LINK_DIRECTORIES
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 
  LINK_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "LINK_DIRECTORY='${dir}'")
endforeach()

# Print DEFINITIONS
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 
    COMPILE_DEFINITIONS)
foreach( dir ${dirs} )
  message( STATUS "DEFINITION: '${dir}'" )
endforeach()