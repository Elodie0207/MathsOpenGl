cmake_minimum_required(VERSION 3.14)
project(MathsOpenGl)

# set enable the debugger (when the debug mode is enable?)
if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    if(NOT (MSVC EQUAL "true"))
        set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")
    endif()
endif()

option(GLM_BUILD_TESTS "" OFF)
add_subdirectory(libs/glm-master)

#option(FREEGLUT_BUILD_SHARED_LIBS "" OFF)
#add_subdirectory(libs/freeglut-3.4.0)
#set(FREEGLUT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/freeglut-3.4.0/include/)

option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
add_subdirectory(libs/glfw-3.3.9)

add_subdirectory(libs/spdlog-1.12.0)

set(IMGUI_GLFW ON)
set(IMGUI_OPENGL3 ON)
set(IMGUI_OPENGL3_LOADER ON)
add_subdirectory(libs/imgui-docking)
add_subdirectory(libs/glad)
# Need to say to imgui where is glfw
target_link_libraries(imgui PUBLIC glfw)

add_subdirectory(Projet)