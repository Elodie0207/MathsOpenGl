cmake_minimum_required(VERSION 3.24)

project(imgui VERSION 1.89.7 DESCRIPTION "The library Dear ImGUI by Omar Cornut in from the master branch.")

option(IMGUI_CUSTOM_LOADER "tell to ImGui you're using an other loader." OFF)

option(IMGUI_ALLEGRO5 "add ALLEGRO5 onto imgui." OFF)
option(IMGUI_ANDROID "add ANDROID onto imgui." OFF)
option(IMGUI_DX9 "add DX9 onto imgui." OFF)
option(IMGUI_DX10 "add DX10 onto imgui." OFF)
option(IMGUI_DX11 "add DX11 onto imgui." OFF)
option(IMGUI_DX12 "add DX12 onto imgui." OFF)
option(IMGUI_GLFW "add GLFW onto imgui." OFF)
option(IMGUI_GLUT "add GLUT onto imgui." OFF)
option(IMGUI_METAL "add METAL onto imgui." OFF)
option(IMGUI_OPENGL2 "add OPENGL2 onto imgui." OFF)
option(IMGUI_OPENGL3 "add OPENGL3 onto imgui." OFF)
option(IMGUI_OPENGL3_LOADER "add OPENGL3 loader onto imgui." OFF)
option(IMGUI_OSX "add OSX onto imgui." OFF)
option(IMGUI_SDL2 "add SDL2 onto imgui." OFF)
option(IMGUI_SDL2_RENDERER "add SDL2 RENDERER onto imgui." OFF)
option(IMGUI_SDL3 "add SDL3 onto imgui." OFF)
option(IMGUI_SDL3_RENDERER "add SDL3 RENDERER onto imgui." OFF)
option(IMGUI_VULKAN "add VULKAN onto imgui." OFF)
option(IMGUI_WGPU "add WGPU onto imgui." OFF)
option(IMGUI_WIN32 "add WIN32 onto imgui." OFF)
option(IMGUI_STDLIB "add STDLIB onto imgui." OFF)

include(GNUInstallDirs)

add_library(imgui)
target_compile_features(imgui PUBLIC cxx_std_17)

target_sources(imgui
        PUBLIC
        imconfig.h
        imgui.cpp
        imgui.h
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
        )

target_include_directories(imgui PUBLIC
        ./
        backends/
        )

if(IMGUI_CUSTOM_LOADER)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=1)
endif ()

if(IMGUI_ALLEGRO5)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_allegro5.cpp
            backends/imgui_impl_allegro5.h
            )
endif()

if(IMGUI_ANDROID)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_android.cpp
            backends/imgui_impl_android.h
            )
endif()

if(IMGUI_DX9)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_dx9.cpp
            backends/imgui_impl_dx9.h
            )
endif()

if(IMGUI_DX10)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_dx10.cpp
            backends/imgui_impl_dx10.h
            )
endif()

if(IMGUI_DX11)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_dx11.cpp
            backends/imgui_impl_dx11.h
            )
endif()

if(IMGUI_DX12)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_dx12.cpp
            backends/imgui_impl_dx12.h
            )
endif()

if(IMGUI_GLFW)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_glfw.cpp
            backends/imgui_impl_glfw.h
            )
endif()

if(IMGUI_GLUT)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_glut.cpp
            backends/imgui_impl_glut.h
            )
endif()

if(IMGUI_METAL)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_metal.mm
            backends/imgui_impl_metal.h
            )
endif()

if(IMGUI_OPENGL2)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_opengl2.cpp
            backends/imgui_impl_opengl2.h
            )
endif()

if(IMGUI_OPENGL3)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_opengl3.cpp
            backends/imgui_impl_opengl3.h
            )
endif()
if(IMGUI_OPENGL3_LOADER)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_opengl3_loader.h
            )
endif()

if(IMGUI_OSX)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_osx.mm
            backends/imgui_impl_osx.h
            )
endif()

if(IMGUI_SDL2)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_sdl2.cpp
            backends/imgui_impl_sdl2.h
            )
endif()

if(IMGUI_SDL2_RENDERER)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_sdlrenderer2.cpp
            backends/imgui_impl_sdlrenderer2.h
            )
endif()

if(IMGUI_SDL3)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_sdl3.cpp
            backends/imgui_impl_sdl3.h
            )
endif()

if(IMGUI_SDL3_RENDERER)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_sdlrenderer3.cpp
            backends/imgui_impl_sdlrenderer3.h
            )
endif()

if(IMGUI_VULKAN)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_vulkan.cpp
            backends/imgui_impl_vulkan.h
            )
endif()

if(IMGUI_WGPU)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_wgpu.cpp
            backends/imgui_impl_wgpu.h
            )
endif()

if(IMGUI_WIN32)
    target_sources(imgui
            PUBLIC
            backends/imgui_impl_win32.cpp
            backends/imgui_impl_win32.h
            )
endif()

if(IMGUI_STDLIB)
    target_sources(imgui
            PUBLIC
            misc/cpp/imgui_stdlib.cpp
            misc/cpp/imgui_stdlib.h
            )
    target_include_directories(imgui PUBLIC misc/cpp/)
endif()

set_target_properties(imgui PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(imgui PROPERTIES SOVERSION 1)