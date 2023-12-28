cmake_minimum_required(VERSION 3.27)
project(MathsOpenGl)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL)
link_directories(${OPENGL_gl_LIBRARY})

include_directories(../libs/glfw/include)
include_directories(../libs/glew/include)

link_directories(../libs/glfw/lib-static-ucrt)
link_directories(../libs/glew/lib/Release/x64)

add_executable(MathsOpenGl main.cpp ../common/GLShader.cpp)
target_link_libraries(MathsOpenGl glfw3 ${OPENGL_gl_LIBRARY} glew32)