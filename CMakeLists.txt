cmake_minimum_required(VERSION 3.10)
project(ImageEditor VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(SFML
                     GIT_REPOSITORY https://github.com/SFML/SFML.git
                     GIT_TAG 3.0.1
                     GIT_SHALLOW ON
                     EXCLUDE_FROM_ALL
                     SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(ImageEditor src/main.cpp src/Window.cpp src/Window.h)
target_link_libraries(ImageEditor PRIVATE SFML::Graphics SFML::Audio)