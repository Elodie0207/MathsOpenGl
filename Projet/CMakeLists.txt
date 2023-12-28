cmake_minimum_required(VERSION 3.27)
project(MathsOpenGl)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
link_directories(${OPENGL_gl_LIBRARY})

include_directories(../libs/glfw/include)
include_directories(../libs/glut/include)

link_directories(../libs/glfw/lib-static-ucrt)
link_directories(../libs/glut/lib/x64)

add_executable(MathsOpenGl main.cpp)
target_link_libraries(MathsOpenGl glfw3 ${OPENGL_gl_LIBRARY} freeglut)