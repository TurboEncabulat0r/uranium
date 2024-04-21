// renderer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "opengl32.lib")

#include <GL/glew.h>
#include "uranium/uranium.h"
#include "imgui.h"
#include <iostream>



void onImgui() {
	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");
	ImGui::End();
}

void onRender() 
{

}



int main(void)
{
	uranium::log(3.2);
	uranium::Initialize();
	uranium::AddRenderCallback(onRender);
	uranium::AddImGUIDrawCallback(onImgui);
	uranium::Loop();
	uranium::Shutdown();
	return 0;
}