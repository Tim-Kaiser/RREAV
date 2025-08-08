include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.9b
    GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(ImGui)

set(IMGUI_DIR "${imgui_SOURCE_DIR}")
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml
    GIT_TAG v3.0
    GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(ImGui-SFML)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glm)