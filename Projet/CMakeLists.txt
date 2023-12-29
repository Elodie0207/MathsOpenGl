cmake_minimum_required(VERSION 3.26)
project(MathsOpenGl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "17")

add_executable(MathsOpenGl
        src/main.cpp
        src/Math.cpp
        include/Math.hpp
        src/Application.cpp
        include/Application.hpp
        include/Macros.hpp
        src/Logger.cpp
        include/Logger.hpp
        include/Assert.hpp
        include/Core.hpp
        src/Parameters.cpp
        include/Parameters.hpp
        src/Camera.cpp
        include/Camera.hpp
        src/Renderer/Poly.cpp
        include/Renderer/Poly.hpp
        include/SystemHelper.hpp
        src/Renderer/DrawObject.cpp
        include/Renderer/DrawObject.hpp
        src/Renderer/Texture.cpp
        include/Renderer/Texture.hpp
        src/UUID.cpp
        include/UUID.hpp
        include/stb_image.h
        src/Renderer/Quad.cpp
        include/Renderer/Quad.hpp
        src/Photoshoop/Tools.cpp
        include/Photoshoop/Tools.hpp
        include/Input.hpp
)

target_include_directories(MathsOpenGl PUBLIC include/ ${FREEGLUT_INCLUDE_DIR})
target_include_directories(MathsOpenGl PRIVATE src/)
target_compile_features(MathsOpenGl PUBLIC cxx_std_17)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
    target_compile_definitions(MathsOpenGl PUBLIC REI_DEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES "[Rr][Ee][Ll][Ee][Aa][Ss][Ee]")
    target_compile_definitions(MathsOpenGl PUBLIC REI_RELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES "[Dd][Ii][Ss][Tt]")
    target_compile_definitions(MathsOpenGl PUBLIC REI_DIST)
endif()
target_compile_definitions(MathsOpenGl PUBLIC REI_SIMPLE_FUNC)

target_link_libraries(MathsOpenGl PUBLIC glfw ${OPENGL_gl_LIBRARY} freeglut_static glm spdlog::spdlog_header_only)