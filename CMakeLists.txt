cmake_minimum_required(VERSION 3.21)
project(rreav)
include(GenerateExportHeader)

if(PROJECT_IS_TOP_LEVEL)
    set(BUILD_SHARED_LIBS OFF)
endif()

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(dependencies)

add_library(rreav STATIC
    src/Interface.cpp 
    src/ShaderManager.cpp 
    src/gl.c
    src/ObjectLoader.cpp
    src/Mesh.cpp
    src/AudioManager.cpp
    )
target_link_libraries(rreav PUBLIC sfml-audio ImGui-SFML::ImGui-SFML glm::glm assimp)

add_library(rreav::rreav ALIAS rreav)

target_include_directories( rreav PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

generate_export_header(
    rreav
    EXPORT_FILE_NAME
    export/rreav/export_rreav.h
)

target_include_directories(
    rreav
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/export>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(demo ${demo})
endif()
