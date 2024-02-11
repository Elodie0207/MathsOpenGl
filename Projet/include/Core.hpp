//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Macros.hpp"
#include "Logger.hpp"
#include "Assert.hpp"
#include "Math.hpp"
#include "UUID.hpp"
#include "Input.hpp"
#include "SmartPtr.hpp"
#include "KeyCodes.hpp"

#include <glad/glad.h>


#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include <imgui.h>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE 1
#endif

#include "GLFW/glfw3.h"