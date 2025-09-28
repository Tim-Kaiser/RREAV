cmake_minimum_required(VERSION 3.16)
project(rreav)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(dependencies)

add_library(rreav STATIC
    include/AudioManager.h
    include/Interface.h
    include/Mesh.h
    include/ObjectLoader.h
    include/ShaderManager.h
    include/Utils.h

    src/Interface.cpp 
    src/ShaderManager.cpp 
    src/gl.c
    src/ObjectLoader.cpp
    src/Mesh.cpp
    src/AudioManager.cpp
    )
target_link_libraries(rreav PUBLIC sfml-audio ImGui-SFML::ImGui-SFML glm::glm assimp)

add_subdirectory(demo)