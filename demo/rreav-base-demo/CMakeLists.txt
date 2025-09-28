set(RESOURCE_PATH_DEST ${CMAKE_CURRENT_BINARY_DIR}/resources)

add_executable(rreav-base-demo
    src/main.cpp
)

target_include_directories(rreav-base-demo PUBLIC include)
target_link_libraries(rreav-base-demo PUBLIC rreav)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${RESOURCE_PATH_DEST}
    DEPENDS ${MY_TARGET}
)
add_dependencies(rreav-base-demo copy_resources)