#pragma once
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"


void InitializeImGUI(GLFWwindow* w, const char* glsl_version);

void ImGUIStartFrame();

void ImGUIEndFrame();

void ImGUIDraw();

void ShutdownImGUI();