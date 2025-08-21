cmake_minimum_required(VERSION 3.16)
project(rreav CXX C)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(dependencies)

set(RESOURCE_PATH_DEST ${CMAKE_CURRENT_BINARY_DIR}/resources)


# TODO: fix this. Compiling under windows with Ninja doesnt create Debug/Release folders, as such this is currently generator specific.
# better way to do this would be to add a custom command for POST_BUILD which copies the resource folder to a Debug/Release folder if it exists.
if(FALSE)
if(CMAKE_HOST_WIN32)
    set(RESOURCE_PATH_DEST ${CMAKE_CURRENT_BINARY_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/resources)
else()
    set(RESOURCE_PATH_DEST ${CMAKE_CURRENT_BINARY_DIR}/resources)
endif()
endif()


add_executable(rreav 
    src/main.cpp 
    src/Interface.cpp 
    src/ShaderManager.cpp 
    src/gl.c
    src/ObjectLoader.cpp
    src/Mesh.cpp
    )


target_include_directories(rreav PRIVATE include)
target_link_libraries(rreav PRIVATE sfml-audio ImGui-SFML::ImGui-SFML glm::glm assimp)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/resources
    ${RESOURCE_PATH_DEST}
    DEPENDS ${MY_TARGET}
)
add_dependencies(rreav copy_resources)