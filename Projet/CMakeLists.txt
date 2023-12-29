cmake_minimum_required(VERSION 3.26)
project(MathsOpenGl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "17")

add_executable(MathsOpenGl
        src/main.cpp
        include/Math.cpp
        include/Math.hpp
)

target_include_directories(MathsOpenGl PUBLIC include/ ${FREEGLUT_INCLUDE_DIR})
target_include_directories(MathsOpenGl PRIVATE src/)
target_compile_features(MathsOpenGl PUBLIC cxx_std_17)

target_link_libraries(MathsOpenGl PUBLIC glfw ${OPENGL_gl_LIBRARY} freeglut_static glm)