cmake_minimum_required(VERSION 3.16)
project(rreav CXX C)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(dependencies)

add_executable(rreav 
    src/main.cpp 
    src/Interface.cpp 
    src/ShaderManager.cpp 
    src/gl.c
    src/ObjectLoader.cpp
    src/Model.cpp
    )

target_include_directories(rreav PRIVATE include)
target_link_libraries(rreav PRIVATE sfml-audio ImGui-SFML::ImGui-SFML glm::glm assimp)
