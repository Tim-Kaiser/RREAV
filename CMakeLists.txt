cmake_minimum_required(VERSION 3.21)
project(rreav)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(dependencies)

add_library(rreav
    include/rreav/AudioManager.h
    include/rreav/Interface.h
    include/rreav/Mesh.h
    include/rreav/ObjectLoader.h
    include/rreav/ShaderManager.h
    include/rreav/Utils.h

    src/Interface.cpp 
    src/ShaderManager.cpp 
    src/gl.c
    src/ObjectLoader.cpp
    src/Mesh.cpp
    src/AudioManager.cpp
    )
add_library(rreav::rreav ALIAS rreav)
target_link_libraries(rreav PUBLIC sfml-audio ImGui-SFML::ImGui-SFML glm::glm assimp)


TARGET_INCLUDE_DIRECTORIES ( rreav PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../include>
)
if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(demo ${demo})
endif()
